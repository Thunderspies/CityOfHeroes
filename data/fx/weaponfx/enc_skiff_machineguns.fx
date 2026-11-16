#########################################################
##	
##

FxInfo
LifeSpan  120



##################################

Condition
	On 	Time
	Time 	1

	Event
		ENAME   Sound
		Type	Local 
		At	WepR
		Sound machgun3 120.0 120.0 .9
	End

End

Condition
	On 	cycle
	Time 	1



	Event
		EName 	Blast1
		Type	local
		At	WepR
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		LifeSpan	1
	End


	Event
		EName 	tracer
		Type	local
		At	WepR
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		LifeSpan	1
		
	End

End


Condition
	On 	cycle
	Time 	3


	Event
		EName 	Blast2
		Type	local
		At	WepL
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		LifeSpan	1
	End


	Event
		EName 	tracer2
		Type	local
		At	WepL
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		LifeSpan	1
		
	End

End			


#Condition
#	On 	cycle
#	Time 	3
#
#
#	Event
#		EName 	GroundBlast
#		Type	local
#		At	BendMystic
#		Geom	FX_MachineGunBurst
#		Bhvr	Behaviors/MachineGunBust.bhvr
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#		LifeSpan	1
#	End
#
#End

End