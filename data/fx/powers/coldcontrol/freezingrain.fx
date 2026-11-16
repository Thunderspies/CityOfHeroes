#########################################################
FxInfo

Condition

	############## The Raining ################################
	Event
		EName	Hookup
		Type	local
		At		Root
		Geom	OverHeadDummy01
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Sound icerain 100 100 .86
	End

	Event
		Type	local
		At		Hookup
		altpiv	1
		
		part	:FreezingRainWaterDrop2.part
		Part	:FreezingSteam.part
		part	:FreezingRainWaterDrop2.part
		Part	:FreezingSteam.part

		sound	freezerain_loop 80 30 .6
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

	############## Spinning geometry to randomize splat positions ##
	Event
		EName 	sleeticesplatSpinsAndOffsets
		Type	start
		At	Root
		Geom	CenterDummy
		BHVR	:SleetSplatRotate.bhvr
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets2
		Type	start
		At	Root
		Geom	CenterDummy
		BHVR	:SleetSplatRotate.bhvr
	End
	
	Event
		EName 	sleeticesplatSpinsAndOffsets3
		Type	start
		At	Root
		Geom	CenterDummy
		BHVR	:SleetSplatRotate.bhvr
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets4
		Type	start
		At	Root
		Geom	CenterDummy
		BHVR	:SleetSplatRotate.bhvr
	End

End

Condition
	On 	Time
	Time	14

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets
		Altpiv	1
		sound	icebreak3 80 70 .96
		
		BHVR	:SleetSplat.bhvr
		Splat	SleetIceSlick.tga
	End

End

Condition
	On 	Time
	Time	15
	
	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets2
		Altpiv	1
		
		BHVR	:SleetSplat.bhvr
		Splat	SleetIceSlick.tga
	End
	
End	

Condition
	On 	Time
	Time	16
	
	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets4
		Altpiv	1
		
		BHVR	:SleetSplat.bhvr
		Splat	SleetIceSlick.tga
	End
	
End

Condition
	On 	Time
	Time	17
	
	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets3
		Altpiv	1
		
		BHVR	:SleetSplat.bhvr
		Splat	SleetIceSlick.tga
	End
	
End	

End		