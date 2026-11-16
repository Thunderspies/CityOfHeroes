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
		
		part1	:Hail.part
		#Part2	:RainCloudLightning1.part
		#part3	:RainCloudLightning2.part
		#part4	:RainCloudLightningMain.part
		#part5	:OutsideLightningRing.part

		PMagnet	root
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		
		part1	:RainCloudDark.part
		part2	:RainCloudDark2.part
		
		
	End

End

End		