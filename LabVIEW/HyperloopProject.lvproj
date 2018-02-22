<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Property Name="SMProvider.SMVersion" Type="Int">201310</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="IOScan.Faults" Type="Str"></Property>
		<Property Name="IOScan.NetVarPeriod" Type="UInt">100</Property>
		<Property Name="IOScan.NetWatchdogEnabled" Type="Bool">false</Property>
		<Property Name="IOScan.Period" Type="UInt">10000</Property>
		<Property Name="IOScan.PowerupMode" Type="UInt">0</Property>
		<Property Name="IOScan.Priority" Type="UInt">9</Property>
		<Property Name="IOScan.ReportModeConflict" Type="Bool">true</Property>
		<Property Name="IOScan.StartEngineOnDeploy" Type="Bool">false</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="ArxPaxGauge2.ctl" Type="VI" URL="../ArxPaxGauge2.ctl"/>
		<Item Name="Bitshifter.vi" Type="VI" URL="../Bitshifter.vi"/>
		<Item Name="BlockDiagramControlEnum.ctl" Type="VI" URL="../BlockDiagramControlEnum.ctl"/>
		<Item Name="CommunicationStatus.ctl" Type="VI" URL="../CommunicationStatus.ctl"/>
		<Item Name="CreateDouble.vi" Type="VI" URL="../CreateDouble.vi"/>
		<Item Name="CreateUint16.vi" Type="VI" URL="../CreateUint16.vi"/>
		<Item Name="CreateUint32.vi" Type="VI" URL="../CreateUint32.vi"/>
		<Item Name="Illini Hyperloop GUI v5.0.vi" Type="VI" URL="../Illini Hyperloop GUI v5.0.vi"/>
		<Item Name="LevitationChart.ctl" Type="VI" URL="../LevitationChart.ctl"/>
		<Item Name="StageEnum.ctl" Type="VI" URL="../StageEnum.ctl"/>
		<Item Name="TCP_NoDelay_Windows.vi" Type="VI" URL="../TCP_NoDelay_Windows.vi"/>
		<Item Name="TemperatureControl.ctl" Type="VI" URL="../TemperatureControl.ctl"/>
		<Item Name="UpdateCommunicationStatus.vi" Type="VI" URL="../UpdateCommunicationStatus.vi"/>
		<Item Name="readTCPDataIn v1.vi" Type="VI" URL="../readTCPDataIn v1.vi"/>
		<Item Name="sendTCPRequest.vi" Type="VI" URL="../sendTCPRequest.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="TCP Get Raw Net Object.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Get Raw Net Object.vi"/>
			</Item>
			<Item Name="wsock32.dll" Type="Document" URL="wsock32.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="HyperloopGUIBuild_Mac" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{FE5AE345-58D9-448F-9F4F-A0DC60FF9D1F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{336DC21E-6A19-4FF2-A896-2B16E08DDAC5}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{44223DAC-1C2F-46DC-AA1A-B74A01034B72}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HyperloopGUIBuild_Mac</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/HyperloopGUIBuild_Mac</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{3F18F044-97BD-4857-8418-1DCB20A44F8F}</Property>
				<Property Name="Bld_version.build" Type="Int">6</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Destination[0].destName" Type="Str">testapp.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/HyperloopGUIBuild_Mac/testapp.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/HyperloopGUIBuild_Mac/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{F26951FB-D55B-4373-B1BD-00FC8D801BB7}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref"></Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HyperloopGUIBuild</Property>
				<Property Name="TgtF_internalName" Type="Str">HyperloopGUIBuild</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2016 </Property>
				<Property Name="TgtF_productName" Type="Str">HyperloopGUIBuild</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{48DAB9BF-D797-4950-9366-360A3A2506BB}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">testapp.exe</Property>
			</Item>
			<Item Name="HyperloopGUIBuild_Windows" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{C48CE74A-CA4B-42F5-B235-962C052F26BA}</Property>
				<Property Name="App_INI_GUID" Type="Str">{643670C5-32FE-4D3D-8FA4-DDDEA01F5554}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E8A0F3D1-B6C4-45BF-9985-BE9E158D00F3}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HyperloopGUIBuild_Windows</Property>
				<Property Name="Bld_compilerOptLevel" Type="Int">0</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/HyperloopGUIBuild_Windows</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{9572037B-313B-4015-9553-40B50AD7AB71}</Property>
				<Property Name="Bld_version.build" Type="Int">16</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Destination[0].destName" Type="Str">testapp.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/HyperloopGUIBuild_Windows/testapp.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/HyperloopGUIBuild_Windows/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{F33E8743-1D41-41F8-BDA4-2055D2539945}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Illini Hyperloop GUI v5.0.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HyperloopGUIBuild</Property>
				<Property Name="TgtF_internalName" Type="Str">HyperloopGUIBuild</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2016 </Property>
				<Property Name="TgtF_productName" Type="Str">HyperloopGUIBuild</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{23F4F507-FEBD-48A3-B053-5FAD978C4120}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">testapp.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
