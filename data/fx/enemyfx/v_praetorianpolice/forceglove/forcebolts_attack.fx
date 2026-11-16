#############################################################
## ForceBolt.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	PushDirection
		Type	Posit
		At	Root
		Lookat	Target
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepL
		SoundNoRepeat 1
		Sound PPD_ForceBolts_Attack_01 200 200 .8
		Sound PPD_ForceBolts_Attack_02 200 200 .8
		Sound PPD_ForceBolts_Attack_03 200 200 .8
		Bhvr	behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare02.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Glow01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Ring01.part
		Lifespan 60
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime2
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName 	Prime3
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime4
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime5
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		BhvrOverride
			TrackRate	6.0
		End
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type    Destroy
		EName 	Prime
	End
End

Condition
	On 	Prime2Hit

	Event
		Type    Destroy
		EName 	Prime2
	End
End

Condition
	On 	Prime3Hit

	Event
		Type    Destroy
		EName 	Prime3
	End
End

Condition
	On 	Prime4Hit

	Event
		Type    Destroy
		EName 	Prime4
	End
End

Condition
	On 	Prime5Hit

	Event
		Type    Destroy
		EName 	Prime5
	End
End

#############################################################

End