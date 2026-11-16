#########################################################
##	template

FxInfo

Input
	Inpname	Hips
End

#####Flags    InheritAlpha

#########################################################


Lifespan	520

Condition
	Event
		EName	DummyChest
		Type	Start
		At	Chest
	End

	Event
		EName	DummyHips
		Type	Start
		At	Hips
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyHips
		Part	:Build_Up_Beams_01.part
		Lifespan	260
	End
End

Condition
	On 	Time
	Time 	80

	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyHips
		Part	:Build_Up_Glow_01.part
		Lifespan	200
	End

	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyHips
		Part	:Beam_Upward_01.part
		Part	:Beam_Upward_02.part
		Lifespan	170
	End

	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyChest
		Part	:Glows_Moving_Hover_01.part
		Lifespan	120
	End

	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyHips
		Part	:Build_Up_Beams_02.part
		Lifespan	220
	End
End

Condition
	On 	Time
	Time 	140

	Event
		Type	Start
		At	DummyHips
		Bhvr	behaviors/CameraShakeHydra.bhvr
	End
End

Condition
	On 	Time
	Time 	160

	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyChest
		Part	:Glows_Moving_Inward_01.part
		Lifespan	80
	End
End



Condition
	On 	Time
	Time 	300
	Event
		EName 	Hydra_Head_Explosion
		Type	Start
		At	DummyChest
		Part	:Burst_01.part
		Lifespan	20
	End

	Event
		EName 	FogRing1
		Type	Start
		At	DummyChest
		bhvr    :CoTEnemyGateSpin3.bhvr
		Part	:CoTEnemyGateRingSmall.part
		part1	:CoT_EnemyPortal.part
		Sound hurricane_loop 100 100 .7
		Lifespan	80

	End

	Event
		EName 	Hurricane
		Type	Start
		At	DummyChest
		bhvr    :CoTEnemyGateSpin1.bhvr
		Part	:CoTEnemyGate01.part
		Lifespan	80
	End

	Event
		EName 	FogRing1
		Type	Start
		At	DummyChest
		bhvr    :CoTEnemyGateSpin1.bhvr
		Lifespan	80
	End

	Event
		EName 	Hydra_Head_Explosion
		Type	start
		At	DummyChest
		Part	:Death_Glow_02.part
		Lifespan	190
	End
End

Condition
	On 	Time
	Time 	490

	Event
		EName 	Hydra_Head_Explosion
		Type	start
		At	DummyChest
		Part	:Death_Twinkle_01.part
		Part	:Death_Twinkle_02.part
		Part	:Death_Twinkle_03.part
		Part	:Death_Twinkle_04.part
		Lifespan	60
	End
End

End