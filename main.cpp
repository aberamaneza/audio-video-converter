//#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"imgui/imgui.h"
#include"imgui/ImGuiFileDialog.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <chrono>
int display_x,display_y;
#define WIN_WIDTH 800
#define WIN_HEIGHT 400
ImVec2 winsize = ImVec2(0,-5);
void setupimgui(GLFWwindow* window);
void cleanimgui();
void RenderUI();
std::string Filedialog();

std::string filepathname;
ImGuiWindowFlags winflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
IGFD::FileDialogConfig config;



        // Set button size
ImVec2 buttonSize(200, 50);
ImVec2 buttonSize2(100, 50);
ImVec2 buttonSize3(100, 50);

static const char* selected_option = "select >";
static const char* options[] =     {
    ".mp4", ".mp3", ".m4a", ".3gp", ".avi", ".mov", ".mkv", ".webm", 
    ".flv", ".wmv", ".mpeg", ".mpg", ".ts", ".ogg", ".rm", ".rmvb", 
    ".mpg4", ".aac", ".wav", ".flac", ".wma", ".alac", ".opus", 
    ".aiff", ".amr", ".mka", ".gif", ".apng", ".m2ts", ".mp2"
};
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE,false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const  GLFWvidmode* mode = glfwGetVideoMode(monitor);
    display_x = mode->width;
    display_y = mode->height;
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"IMGUI",NULL,NULL);
    glfwSetWindowPos(window,0,30);


    glfwMakeContextCurrent(window);
	glfwSwapInterval(1);  // 1 يعني تفعيل VSync

    setupimgui(window);
    config.filePathName = "."; 
    ImGuiIO& io = ImGui::GetIO();
    // تحميل الخط Arial بحجم 18 بكسل
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 18.0f);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // تعيين لون الخلفية للمسح.
        glClear(GL_COLOR_BUFFER_BIT);    // مسح محتويات الشاشة باستخدام لون الخلفية.

        RenderUI();
        
        //glfwGetFramebufferSize(window,&display_x,&display_y);
        //glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    cleanimgui();
    glfwTerminate();
    return 0;

}




void setupimgui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();(void)io;

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");

}
void cleanimgui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void RenderUI(){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::SetNextWindowPos(winsize,ImGuiCond_Always);
    ImGui::NewFrame();
    
    ImGui::Begin("mmm",nullptr,winflags);
    Filedialog();
    
    ImGui::SetCursorPos(ImVec2(300, 100));
    
    // تخصيص حجم الإدخال
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 20)); // حشو عمودي
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10)); // مسافات بين العناصر
    ImGui::PushItemWidth(100); // عرض الإدخال

    static char buffer[128] = "";
    ImGui::InputText(" ", buffer, IM_ARRAYSIZE(buffer));

    ImGui::PopItemWidth();
    ImGui::PopStyleVar(2); // إعادة القيم الافتراضية
    ImGui::SetCursorPos(ImVec2(470, 100));
        if (ImGui::Button(selected_option,buttonSize2)) {
            ImGui::OpenPopup("SelectOptionPopup");
        }

        // Create the combo box popup
        if (ImGui::BeginPopup("SelectOptionPopup")) {
            for (const char* option : options) {
                if (ImGui::Selectable(option, option == selected_option)) {
                    selected_option = option; // Update the selected option
                }
            }
            ImGui::EndPopup();
        }
    ImGui::SetCursorPos(ImVec2(590,100));
    if(ImGui::Button("export",buttonSize3))
    {
        std::string comande = "start ffmpeg/ffmpeg.exe -i "+ Filedialog() +" "+buffer+selected_option; 
        std::cout<<comande<<std::endl;
        system(comande.c_str());
    }
    //ImGui::Text("hello");
    ImGui::End();
    ImGui::Render();
}
std::string Filedialog()
{
    
    ImGui::SetCursorPos(ImVec2(50, 100));
    if(ImGui::Button("open file",buttonSize)){
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*",config);
    }
    if(ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")){
        if(ImGuiFileDialog::Instance()->IsOk()){
            std::string filepath = ImGuiFileDialog::Instance()->GetCurrentPath();
            filepathname = ImGuiFileDialog::Instance()->GetFilePathName();
        }
        
        ImGuiFileDialog::Instance()->Close();
    }
    return filepathname;
}