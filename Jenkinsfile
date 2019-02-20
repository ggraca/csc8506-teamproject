node {
	stage 'Checkout'
		checkout scm

	stage 'Build Release Win32'
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Release /p:Platform=\"Win32\""

	stage 'Build Release x64'
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Release /p:Platform=\"x64\""

	stage 'Build Debug Win32'
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Debug /p:Platform=\"Win32\""

	stage 'Build Debug x64'
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Debug /p:Platform=\"x64\""

	stage 'Archive'
		archive 'TeamProject/bin/Release/**'
		archive 'TeamProject/bin/Debug/**'

}
