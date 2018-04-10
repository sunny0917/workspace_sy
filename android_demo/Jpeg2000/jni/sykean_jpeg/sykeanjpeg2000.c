
#include "sykeanjpeg2000.h"

//int DebugBufferChar(const char* pszSvBufPath, unsigned char* pBuffer, int iWidth, int iHeight)
//{
//  if (pszSvBufPath == NULL)
//  {
//      return 1;
//  }
//
//  if (pBuffer == NULL)
//  {
//      return 1;
//  }
//
//  FILE* file = NULL;
//
//  file = fopen(pszSvBufPath, "w");
//
//  if (file == NULL)
//  {
//      return -1;
//  }
//
//  int write_length = fwrite(pBuffer, 1, iWidth * iHeight, file);
//
//  fclose(file);
//  file = NULL;
//
//  return 0;
//}

opj_image_t* buffertoimage(unsigned char* pBuffer, int iWidth, int iHeight, opj_cparameters_t *parameters)
{
    int subsampling_dx = parameters->subsampling_dx;
    int subsampling_dy = parameters->subsampling_dy;
    opj_image_cmptparm_t cmptparm[3];

    int W = iWidth;
    int H = iHeight;

    int i, j, w, h, numcomps = 1;
    w = iWidth;
    h = iHeight;
    OPJ_COLOR_SPACE color_space = OPJ_CLRSPC_GRAY;
    opj_image_t * image = NULL;

    /* initialize image components */
    memset(&cmptparm[0], 0, 3 * sizeof(opj_image_cmptparm_t));
    for (i = 0; i < numcomps; i++)
    {
        cmptparm[i].prec = 8;
        cmptparm[i].bpp = 8;
        cmptparm[i].sgnd = 0;
        cmptparm[i].dx = (OPJ_UINT32)subsampling_dx;
        cmptparm[i].dy = (OPJ_UINT32)subsampling_dy;
        cmptparm[i].w = (OPJ_UINT32)w;
        cmptparm[i].h = (OPJ_UINT32)h;
    }
    /* create the image */
    image = opj_image_create((OPJ_UINT32)numcomps, &cmptparm[0], color_space);
    if (!image)
    {
        return NULL;
    }

    /* set image offset and reference grid */
    image->x0 = (OPJ_UINT32)parameters->image_offset_x0;
    image->y0 = (OPJ_UINT32)parameters->image_offset_y0;
    image->x1 = !image->x0 ? (OPJ_UINT32)(w - 1) * (OPJ_UINT32)subsampling_dx + 1 : image->x0 + (OPJ_UINT32)(w - 1) * (OPJ_UINT32)subsampling_dx + 1;
    image->y1 = !image->y0 ? (OPJ_UINT32)(h - 1) * (OPJ_UINT32)subsampling_dy + 1 : image->y0 + (OPJ_UINT32)(h - 1) * (OPJ_UINT32)subsampling_dy + 1;

    /* set image data */
    for (j = 0; j < W * H; j++)
    {
        image->comps[0].data[j] = pBuffer[j];
    }

    return image;
}

static int opj_close_from_buffer(void * p_buffer, OPJ_SIZE_T p_nb_bytes, stream_buffer* p_membuffer)
{
    return 0;
}

static OPJ_UINT64 opj_get_data_length_from_buffer(stream_buffer* p_membuffer)
{
    return (OPJ_UINT64)(p_membuffer->cur - p_membuffer->mem_buffer);
}

static OPJ_SIZE_T opj_read_from_buffer(void * p_buffer, OPJ_SIZE_T p_nb_bytes, stream_buffer* p_membuffer)
{
    return 0;
}

static OPJ_SIZE_T opj_write_from_buffer(void * p_buffer, OPJ_SIZE_T p_nb_bytes, stream_buffer* p_membuffer)
{
    memcpy(p_membuffer->cur, p_buffer, p_nb_bytes);
    p_membuffer->cur += p_nb_bytes;

    return p_nb_bytes;
}

static OPJ_OFF_T opj_skip_from_buffer(OPJ_OFF_T p_nb_bytes, stream_buffer * p_membuffer)
{
    p_membuffer->cur += p_nb_bytes;

    return p_nb_bytes;
}

static OPJ_BOOL opj_seek_from_buffer(OPJ_OFF_T p_nb_bytes, stream_buffer * p_membuffer)
{
    p_membuffer->cur = p_membuffer->mem_buffer + p_nb_bytes;

    return OPJ_TRUE;
}

opj_stream_t* OPJ_CALLCONV opj_stream_create_default_buffer_stream(stream_buffer* p_membuffer,
    OPJ_BOOL p_is_read_stream)
{
    return opj_stream_create_buffer_stream(p_membuffer, OPJ_J2K_STREAM_CHUNK_SIZE, p_is_read_stream);
}

opj_stream_t* OPJ_CALLCONV opj_stream_create_buffer_stream(stream_buffer* p_membuffer,
    OPJ_SIZE_T p_size, OPJ_BOOL p_is_read_stream)
{
    opj_stream_t* l_stream = 00;

    if (p_membuffer == NULL) {
        return NULL;
    }

    l_stream = opj_stream_create(p_size, p_is_read_stream);

    if (!l_stream) {
        free(p_membuffer->mem_buffer);
        free(p_membuffer);
        return NULL;
    }

    opj_stream_set_user_data(l_stream, p_membuffer, (opj_stream_free_user_data_fn)opj_close_from_buffer);
    opj_stream_set_user_data_length(l_stream, opj_get_data_length_from_buffer(p_membuffer));
    opj_stream_set_read_function(l_stream, (opj_stream_read_fn)opj_read_from_buffer);
    opj_stream_set_write_function(l_stream, (opj_stream_write_fn)opj_write_from_buffer);
    opj_stream_set_skip_function(l_stream, (opj_stream_skip_fn)opj_skip_from_buffer);
    opj_stream_set_seek_function(l_stream, (opj_stream_seek_fn)opj_seek_from_buffer);

    return l_stream;
}

int OPJ_CALLCONV SykeanJPEG2000Compress(unsigned char* pBufferIn, int iWidth, int iHeight,
    stream_buffer* pBufferOut, int* iBufferOutLength, int iDestBytes)
{
    if (pBufferIn == NULL || pBufferOut == NULL)
    {
        return -11;
    }

    if (iDestBytes < 1024)
    {
        return -12;
    }

    if ((iWidth*iHeight) / iDestBytes< 2)
    {
        return -13;
    }

    opj_cparameters_t parameters;   /* compression parameters */

    opj_stream_t* l_stream = 00;
    opj_codec_t* l_codec = 00;
    opj_image_t* image = NULL;

    OPJ_BOOL bSuccess;
    //OPJ_BOOL bUseTiles = OPJ_FALSE;

    /* set encoding parameters to default values */
    opj_set_default_encoder_parameters(&parameters);

    /* parse input and get user encoding parameters */
    parameters.tcp_mct = (char)255; /* This will be set later according to the input image or the provided option */
    parameters.decod_format = 12;
    parameters.cod_format = 1;
    parameters.tcp_numlayers = 1;

    OPJ_FLOAT32 inSize = iWidth*iHeight;
    OPJ_FLOAT32 outSize = iDestBytes;
    parameters.tcp_rates[0] = inSize / outSize;
    parameters.cp_disto_alloc = 1;

    /* Create comment for codestream */
    //if (parameters.cp_comment == NULL) {
    //  const char comment[] = "Created by Sykean OpenJPEG version";
    //  const size_t clen = strlen(comment);
    //  parameters.cp_comment = (char*)malloc(clen + 1);
    //  sprintf(parameters.cp_comment, "%s", comment);
    //}

    /* Encoding image */
    image = buffertoimage(pBufferIn, iWidth, iHeight, &parameters);

    if (!image)
    {
        return -14;
    }

    /* Decide if MCT should be used */
    if (parameters.tcp_mct == (char)255)
    { /* mct mode has not been set in commandline */
        parameters.tcp_mct = (image->numcomps >= 3) ? 1 : 0;
    }

    /* encode the destination image */
    /* ---------------------------- */
    switch (parameters.cod_format) {
        case J2K_CFMT:  /* JPEG-2000 codestream */
        {
            /* Get a decoder handle */
            l_codec = opj_create_compress(OPJ_CODEC_J2K);
            break;
        }
        case JP2_CFMT:  /* JPEG 2000 compressed image data */
        {
            /* Get a decoder handle */
            l_codec = opj_create_compress(OPJ_CODEC_JP2);
            break;
        }
    }

    opj_setup_encoder(l_codec, &parameters, image);

    l_stream = opj_stream_create_default_buffer_stream(pBufferOut, OPJ_FALSE);

    /* encode the image */
    bSuccess = opj_start_compress(l_codec, image, l_stream);
    if (bSuccess)
    {
        bSuccess = bSuccess && opj_encode(l_codec, l_stream);
    }
    if (bSuccess)
    {
        bSuccess = bSuccess && opj_end_compress(l_codec, l_stream);
    }

    if (!bSuccess)
    {
        opj_stream_destroy(l_stream);
        opj_destroy_codec(l_codec);
        opj_image_destroy(image);

        return -15;
    }

    *iBufferOutLength = pBufferOut->cur - pBufferOut->mem_buffer;

    //fprintf(stdout, "[INFO] Generated outfile %s\n", parameters.outfile);

    /* close and free the byte stream */
    opj_stream_destroy(l_stream);

    /* free remaining compression structures */
    opj_destroy_codec(l_codec);

    /* free image data */
    opj_image_destroy(image);

    return 0;
}
