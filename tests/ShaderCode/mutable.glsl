    vec4 tex = texture(atlas,uv);
    float a = tex.a*color.a;
    outColor = vec4(color.rgb*tex.rgb,a);
