/*
 * copyright (c) 2020 Mik Bry <mik@mikbry.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

static av_cold int mediacodec_encode_init(AVCodecContext *avctx)
{
    /* VTEncContext    *vtctx = avctx->priv_data;
    CFBooleanRef    has_b_frames_cfbool;
    int             status;

    pthread_once(&once_ctrl, loadVTEncSymbols);

    pthread_mutex_init(&vtctx->lock, NULL);
    pthread_cond_init(&vtctx->cv_sample_sent, NULL);

    vtctx->session = NULL;
    status = vtenc_configure_encoder(avctx);
    if (status) return status;

    status = VTSessionCopyProperty(vtctx->session,
                                   kVTCompressionPropertyKey_AllowFrameReordering,
                                   kCFAllocatorDefault,
                                   &has_b_frames_cfbool);

    if (!status && has_b_frames_cfbool) {
        //Some devices don't output B-frames for main profile, even if requested.
        vtctx->has_b_frames = CFBooleanGetValue(has_b_frames_cfbool);
        CFRelease(has_b_frames_cfbool);
    }
    avctx->has_b_frames = vtctx->has_b_frames; */

    return 0;
}


static av_cold int mediacodec_encode_close(AVCodecContext *avctx)
{
    /* VTEncContext *vtctx = avctx->priv_data;

    pthread_cond_destroy(&vtctx->cv_sample_sent);
    pthread_mutex_destroy(&vtctx->lock);

    if(!vtctx->session) return 0;

    VTCompressionSessionCompleteFrames(vtctx->session,
                                       kCMTimeIndefinite);
    clear_frame_queue(vtctx);
    CFRelease(vtctx->session);
    vtctx->session = NULL;

    if (vtctx->color_primaries) {
        CFRelease(vtctx->color_primaries);
        vtctx->color_primaries = NULL;
    }

    if (vtctx->transfer_function) {
        CFRelease(vtctx->transfer_function);
        vtctx->transfer_function = NULL;
    }

    if (vtctx->ycbcr_matrix) {
        CFRelease(vtctx->ycbcr_matrix);
        vtctx->ycbcr_matrix = NULL;
    } */

    return 0;
}

static av_cold int mediacodec_encode_frame(
    AVCodecContext *avctx,
    AVPacket       *pkt,
    const AVFrame  *frame,
    int            *got_packet)
{
    /* VTEncContext *vtctx = avctx->priv_data;
    bool get_frame;
    int status;
    CMSampleBufferRef buf = NULL;
    ExtraSEI *sei = NULL;

    if (frame) {
        status = vtenc_send_frame(avctx, vtctx, frame);

        if (status) {
            status = AVERROR_EXTERNAL;
            goto end_nopkt;
        }

        if (vtctx->frame_ct_in == 0) {
            vtctx->first_pts = frame->pts;
        } else if(vtctx->frame_ct_in == 1 && vtctx->has_b_frames) {
            vtctx->dts_delta = frame->pts - vtctx->first_pts;
        }

        vtctx->frame_ct_in++;
    } else if(!vtctx->flushing) {
        vtctx->flushing = true;

        status = VTCompressionSessionCompleteFrames(vtctx->session,
                                                    kCMTimeIndefinite);

        if (status) {
            av_log(avctx, AV_LOG_ERROR, "Error flushing frames: %d\n", status);
            status = AVERROR_EXTERNAL;
            goto end_nopkt;
        }
    }

    *got_packet = 0;
    get_frame = vtctx->dts_delta >= 0 || !frame;
    if (!get_frame) {
        status = 0;
        goto end_nopkt;
    }

    status = vtenc_q_pop(vtctx, !frame, &buf, &sei);
    if (status) goto end_nopkt;
    if (!buf)   goto end_nopkt;

    status = vtenc_cm_to_avpacket(avctx, buf, pkt, sei);
    if (sei) {
        if (sei->data) av_free(sei->data);
        av_free(sei);
    }
    CFRelease(buf);
    if (status) goto end_nopkt;

    *got_packet = 1;
    return 0;

end_nopkt:
    av_packet_unref(pkt);
    return status; */

    return 0;
}

static const AVClass h264_mediacodec_class = {
 .class_name = "h264_mediacodec",
 .item_name  = av_default_item_name,
 .option     = h264_options,
 .version    = LIBAVUTIL_VERSION_INT,
};

AVCodec ff_h264_mediacodec_encoder = {
 .name             = "h264_mediacodec",
 .long_name        = NULL_IF_CONFIG_SMALL("Android MediaCodec H.264 Encoder"),
 .type             = AVMEDIA_TYPE_VIDEO,
 .id               = AV_CODEC_ID_H264,
 .priv_data_size   = sizeof(VTEncContext),
 .pix_fmts         = avc_pix_fmts,
 .init             = mediacodec_encode_init,
 .encode2          = mediacodec_encode_frame,
 .close            = mediacodec_encode_close,
 .capabilities     = AV_CODEC_CAP_DELAY | AV_CODEC_CAP_AVOID_PROBING | AV_CODEC_CAP_HARDWARE,
 .priv_class       = &h264_mediacodec_class,
 .caps_internal    = FF_CODEC_CAP_INIT_THREADSAFE |
                     FF_CODEC_CAP_INIT_CLEANUP,
};
