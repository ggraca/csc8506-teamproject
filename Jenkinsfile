node {
	stage 'Checkout'
		checkout scm

	stage 'Build'
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Release /p:Platform=\"x64\""
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Release /p:Platform=\"Win32\""
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Debug /p:Platform=\"x64\""
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Debug /p:Platform=\"Win32\""

	stage 'Archive'
		archive 'TeamProject/bin/Release/**'

}
