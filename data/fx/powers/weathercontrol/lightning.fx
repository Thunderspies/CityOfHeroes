#########################################################
##	RadiationAura
FxInfo


Input  
	InpName	root
End

##################################
Condition
	On	Time
	Time	0
	Event
		EName	Hookup
		Type	local
		At	root
		Geom	OverHeadDummy
		
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		
		part1	:RainCloudLightning.part
		Part2	:RainCloudLightning1.part
		part3	:RainCloudRain.part
		part4	:RainCloudLightningMain.part
		part5	:OutsideLightningRing.part

		PMagnet	root
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		
		part1	:RainCloudDark.part
		part2	:RainCloudDark2.part
		part3	:RainCloudDark3.part
		part4	:RainCloudFlash.part
	End

End

End		