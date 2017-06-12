#include"Renderer.h"
Renderer::Renderer()
{
    _direct = NULL;
    _camera = NULL;
    _shader = NULL;
    _light  = NULL;
}
Renderer::Renderer(const Renderer& renderer)
{

}
Renderer::~Renderer()
{

}
void Renderer::init(HWND hwnd, int screenWidth, int screenHeight)
{
    //▼다렉
    _direct = new Direct;
    if (!_direct) MessageBox(NULL, L"new Direct", L"Error", NULL);
    _direct->init(hwnd, FULL_SCREEN, VSYNC_ENABLED, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
    
    //▼카메라
    _camera = new Camera;
    if (!_camera) MessageBox(NULL, L"new Camera", L"Error", NULL);
    _camera->setPos(D3DXVECTOR3(0, 0, -1250));
    _camera->setRot(D3DXVECTOR3(45, 0, 0));


    /*
        추후에 _fbx 포인터들을
        _object애들 멤버변수로 넘겨가지고,
        render시 _object에서 바로 쓸 수 있도록 깔끔하게 디자인해야겠다
    */
    //▼모델
    //땅
    _fbx[0] = new FBX;
    if (!_fbx[0]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[0]->init(_direct->getDevice(), "Resource/FBX/Ground.fbx", L"Resource/DDS/Ground.dds");

    _fbx[1] = new FBX;
    if (!_fbx[1]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[1]->init(_direct->getDevice(), "Resource/FBX/wall.fbx", L"Resource/DDS/wall.dds");

    //스카이박스
    _fbx[2] = new FBX;
    if (!_fbx[2]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[2]->init(_direct->getDevice(), "Resource/FBX/skybox.fbx", L"Resource/DDS/skybox.dds");
    //오브젝트 게이트
    _fbx[3] = new FBX;
    if (!_fbx[3]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[3]->init(_direct->getDevice(), "Resource/FBX/gate.fbx", L"Resource/DDS/gate.dds");
    //오브젝트 바위
    _fbx[4] = new FBX;
    if (!_fbx[4]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[4]->init(_direct->getDevice(), "Resource/FBX/stone.fbx", L"Resource/DDS/stone.dds");
    //오브젝트 미니언
    _fbx[5] = new FBX;
    if (!_fbx[5]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[5]->init(_direct->getDevice(), "Resource/FBX/minion.fbx", L"Resource/DDS/minion.dds");

    //오브젝트PC
    _fbx[6] = new FBX;
    if (!_fbx[6])MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[6]->init(_direct->getDevice(), "Resource/FBX/hengelBlack.fbx", L"Resource/DDS/hengelBlack.dds");


    //▼오브젝트 매니저
    Object* object;
    // 땅 * 1
    object= new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 벽 * 1
    object= new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 스카이박스 * 1
    object = new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 게이트 * 4
    object = new ObjectGate();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(-600, 0, -600);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(600, 0, -600);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(0, 0, 800);
    _objectManager.addObject(object);
    // 바위 * N
    for(int i=0;i<100;i++)
    {
        object= new ObjectStone();
        SRAND_SEED(i);
        object->setPos(RANDOM_E(400, 1000), 0, RANDOM_E(400, 1000));
        object->setRotY(RANDOM_M(0, 360));
        static float r = 1400;
        if (r < object->getPosX())
            object->setPosX(r);
        if (object->getPosX() < -1*r)
            object->setPosX(-1 * r);
        if (r < object->getPosZ())
            object->setPosZ(r);
        if (object->getPosZ() < -1 * r)
            object->setPosZ(-1 * r);

        _objectManager.addObject(object);
    }
    // 미니언 * N
    for (int i = 0; i < 200; i++)
    {
        object = new ObjectMinion();
        object->setPos(0, 0, 0);
        _objectManager.addObject(object);
    }
    // PC * 1
    object = new ObjectPC();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);


    //▼셰이더
    _shader = new AmbientLightShader;
    if (!_shader) MessageBox(NULL, L"new LightShader", L"Error", NULL);
    _shader->init(_direct->getDevice(), hwnd);
    
    //▼조명
    _light = new Light;
    if (!_light) MessageBox(NULL, L"new Light", L"Error", NULL);
    _light->setDiffuseColor(0.2f, 0.2f, 0.2f, 1.0f);
    _light->setAmbientColor(0.9f, 0.9f, 0.9f, 1.0f);
    _light->setDirection(0.f, -1.f, 0.f);
#ifdef _DEBUG
    printf("Renderer::init();\n");
#endif
}
void Renderer::release()
{
    if (_light)
    {
        delete _light;
        _light = NULL;
    }
    if (_shader)
    {
        _shader->release();
        delete _shader;
        _shader = NULL;
    }

    for (int i = 0; i < 10; i++)
    {
        if (_fbx[i])
        {
            _fbx[i]->release();
            delete _fbx[i];
            _fbx[i] = NULL;
        }
    }

    if (_camera)
    {
        delete _camera;
        _camera = NULL;
    }    
    if (_direct)
    {
        _direct->release();
        delete _direct;
        _direct = NULL;
    }
}
void Renderer::render()
{
    //▼백버퍼 클리어
    _direct->clearBuffer(0.1f, 0.1f, 0.4f, 1.0f);
    //▼일련의 렌더과정
    // Model한테 다렉DC를 넣어서
    // DC::IASetVertexBuffers(모델 버텍스 버퍼);
    // DC::IASetIndexBuffer(모델 인덱스 버퍼);
    // DC::IASetPrimitiveTopology(삼각형);

    // 땅
    _fbx[0]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[0]->getIndexCount(),
        _objectManager.getWorldMatrix(0),// 중요..
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[0]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // 벽
    _fbx[1]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[1]->getIndexCount(),
        _objectManager.getWorldMatrix(1),// 중요..
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[1]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // 스카이박스
    _fbx[2]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[2]->getIndexCount(),
        _objectManager.getWorldMatrix(2),// 중요..
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[2]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // 게이트
    _fbx[3]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(3),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(4),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(5),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(6),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // 스톤
    _fbx[4]->render(_direct->getDeviceContext());
    for(int i=7;i<107;i++)
    {
        _shader->render(
            _direct->getDeviceContext(),
            _fbx[4]->getIndexCount(),
            _objectManager.getWorldMatrix(i),
            _camera->getMtxView(), _direct->getMtxProjection(),
            _fbx[4]->getTexture(),
            _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    }

    // 미니언
    _fbx[5]->render(_direct->getDeviceContext());
    for(int i=107;i<307;i++)
    {
        _shader->render(
            _direct->getDeviceContext(),
            _fbx[5]->getIndexCount(),
            _objectManager.getWorldMatrix(i),
            _camera->getMtxView(), _direct->getMtxProjection(),
            _fbx[5]->getTexture(),
            _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    }
    // pc
    _fbx[6]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[6]->getIndexCount(),
        _objectManager.getWorldMatrix(307),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[6]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    

    //▼백버퍼 스왑체인
    _direct->swapchain();
}
void Renderer::update()
{
    _objectManager.update();
    _camera->update();
}