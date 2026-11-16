#############################################################
## MaceMastery_ShatterArmor.fx
#############################################################

FxInfo

LifeSpan 50

## FALLBACK NODES ###########################################################

Input
	Inpname Chest
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Chest
		BHVR	Behaviors\CameraShake01Subtle.bhvr
		Part1	powers\MedievalWeapons\MaceFlash.part
		Part4	powers\MedievalWeapons\MaceHitGlow01Sml.part
		Part4	powers\MedievalWeapons\MaceHitStreaks.part
		Sound Axe5 80.0 80.0 0.8
		Lifespan 2
	End

	Event
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MartialArtsHit.part
		Part2	Powers\MartialArts\MartialArtsHit1.part
		Part3	Powers\MartialArts\MartialArtsHit2.part
	End

	Event
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01Sml.part
		Part2	Powers\MartialArts\MAHitStar02Sml.part
		Part3	Powers\MartialArts\MAHitStar03Sml.part
		Part5	Powers\MartialArts\MAHitRays01Sml.part
	End

	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
	End


End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Neck
		Part	powers\MedievalWeapons\ShatterCracks01.part
		POther	Waist
		Lifespan 1
	End

	Event
		Type 	Local
		At	Chest
		Part	powers\MedievalWeapons\ShatterCracks01.part
		POther	Hips
		Lifespan 1
	End

	Event
		Type 	Local
		At	UArmL
		Part	powers\MedievalWeapons\ShatterCracks01.part
		POther	LArmL
		Lifespan 1
	End

	Event
		Type 	Local
		At	UArmR
		Part	powers\MedievalWeapons\ShatterCracks01.part
		POther	LArmR
		Lifespan 1
	End

	Event
		Type 	Local
		At	LArmL
		Part	powers\MedievalWeapons\ShatterCracks01.part
		POther	WepL
		Lifespan 1
	End

	Event
		Type 	Local
		At	LArmR
		Part	powers\MedievalWeapons\ShatterCracks01.part
		POther	WepR
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type 	Local
		At	Neck
		Part	powers\MedievalWeapons\ShatterFlash01.part
		Part	powers\MedievalWeapons\ShatterParticles01.part
		Sound ShatterArmor1 100.0 100.0 1.0
		POther	Waist
		Lifespan 2
	End
End

Condition
	On	Time
	Time	22

	Event
		Type 	Local
		At	Chest
		Part	powers\MedievalWeapons\ShatterFlash01.part
		Part	powers\MedievalWeapons\ShatterParticles01.part
		POther	Hips
		Lifespan 2
	End

	Event
		Type 	Local
		At	UArmL
		Part	powers\MedievalWeapons\ShatterFlash01.part
		Part	powers\MedievalWeapons\ShatterParticles01.part
		POther	LArmL
		Lifespan 2
	End

	Event
		Type 	Local
		At	UArmR
		Part	powers\MedievalWeapons\ShatterFlash01.part
		Part	powers\MedievalWeapons\ShatterParticles01.part
		POther	LArmR
		Lifespan 2
	End
End

Condition
	On	Time
	Time	24

	Event
		Type 	Local
		At	LArmL
		Part	powers\MedievalWeapons\ShatterFlash01.part
		Part	powers\MedievalWeapons\ShatterParticles01.part
		POther	WepL
		Lifespan 2
	End

	Event
		Type 	Local
		At	LArmR
		Part	powers\MedievalWeapons\ShatterFlash01.part
		Part	powers\MedievalWeapons\ShatterParticles01.part
		POther	WepR
		Lifespan 2
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Chance	0.6
	Repeat		8
	RepeatJitter	2

	Event
		EName	Sparkup
		Type 	Start
		At	hips
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplodeUP.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat		5
	RepeatJitter	1

	Event
		EName	Sparkdown
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat		7
	RepeatJitter	6

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat		6
	RepeatJitter	4

	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat		10
	RepeatJitter	2

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End

#############################################################

End