#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadowMap;  // The shadow map texture
uniform float time;           // 动态变化的时间参数
uniform vec2 resolution;      // 屏幕的分辨率，用于标准化纹理坐标

vec2 random2(vec2 p) {
    return fract(sin(vec2(
        dot(p, vec2(127.1, 311.7)),
        dot(p, vec2(269.5, 183.3))
    )) * 43758.5453);
}

// 经典的 Perlin 噪声函数
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    // 四角的随机值
    float a = dot(random2(i), f);
    float b = dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0));
    float c = dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0));
    float d = dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0));

    // 平滑插值
    f = f * f * (3.0 - 2.0 * f);
    
    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// 分形 Brownian 运动 (fBm) 来生成更复杂的噪声
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.6;
    float frequency = 0.4;

    // 增加八度数量以获得更多细节
    for(int i = 0; i < 12; i++) {
        value += amplitude * noise(p * frequency);
        frequency *= 1.5;
        amplitude *= 0.65;
    }
    return value;
}

void main() {
    // 调整云的颜色，使用略微偏蓝的白色使其更自然
    vec3 brightCloud = vec3(1.5, 1.5, 1.5);      // 最亮的部分纯白
    vec3 dimCloud = vec3(0.8, 0.8, 0.85);      // 较淡的部分略带蓝

    // 基础云层
    vec2 cloudPosition = TexCoords * 6.0;  // 放大纹理细节
    cloudPosition.x += time * 0.04;  // 云的移动速度, 使用time来移动云
    float cloud1 = fbm(cloudPosition);  // 使用 fBm 生成云的形态，在main()中添加多层云
    
    // 旋转的细节层
    vec2 detailPosition = TexCoords * 12.0;   // 更大的缩放产生更小的细节
    float angle1 = 0.4;
    detailPosition = vec2(
        detailPosition.x * cos(angle1) - detailPosition.y * sin(angle1),
        detailPosition.x * sin(angle1) + detailPosition.y * cos(angle1)
    );
    detailPosition.x -= time * 0.06;         // 反向移动
    float cloud2 = fbm(detailPosition);

    // 第二个旋转角度的细节层
    vec2 detail2Position = TexCoords * 9.0;    // 更小的缩放产生更大的变化
    float angle2 = -0.2;
    //largePosition.x += time * 0.02;          // 较慢移动
    //largePosition += vec2(time * 0.05, sin(time * 0.01) * 0.5);
    detail2Position = vec2(
        detail2Position.x * cos(angle2) - detail2Position.y * sin(angle2),
        detail2Position.x * sin(angle2) + detail2Position.y * cos(angle2)
    );
    detail2Position += vec2(time * 0.05, sin(time * 0.03));
    float cloud3 = fbm(detail2Position);

    // 大尺度变化层
    vec2 largePosition = TexCoords * 3.0;
    largePosition += vec2(sin(time * 0.03) * 0.2, cos(time * 0.025) * 0.2);
    float cloud4 = fbm(largePosition);
    
    // 混合多层云
    //float cloud = cloud1 * 0.7 +             // 基础云层
                // cloud2 * 0.2 +              // 细节
                // cloud3 * 0.1;               // 大尺度变化
    // 更复杂的混合
    float baseCloud = mix(cloud1, cloud4, 0.4);     // 混合大尺度变化
    float details = mix(cloud2, cloud3, 0.5);       // 混合细节
    float cloud = mix(baseCloud, details, 0.3);     // 最终混合

    // 增加云的最终亮度和对比度
    cloud = pow(cloud, 0.7);                // 提升高值区域
    cloud = cloud * 1.0 + 0.3;              // 调整整体亮度

    // 分离更多层次的云
    float veryDenseCloud = smoothstep(0.55, 0.65, cloud);    // 最浓密的部分
    float denseCloud = smoothstep(0.35, 0.55, cloud);       // 中等密度
    float lightCloud = smoothstep(0.15, 0.55, cloud);       // 稀疏部分

    // 混合疏密不同的区域
    float cloudMix = veryDenseCloud * 1.8 +                 // 最浓密部分权重更大
                    denseCloud * 0.4 +
                    lightCloud * 0.1;

    // 让云的形状更加柔和，设置阈值来决定云的"浓密"程度
    // float cloudThreshold = smoothstep(0.4, 0.6, cloud);

    // 强化白色
    float whiteness = pow(veryDenseCloud, 0.4);              // 降低指数，让白色更容易出现
    vec3 cloudColor = mix(dimCloud, brightCloud, whiteness);

    // 云的透明度用于混合
    //float alpha = cloudThreshold * 0.5;
    float alpha = min(cloudMix * 1.5, 1.0);                 // 增加整体不透明度但限制最大值

    // 限制最终颜色的范围
    vec3 finalColor = min(cloudColor, vec3(1.0));          // 确保不会过亮

    FragColor = vec4(cloudColor, alpha);
}
