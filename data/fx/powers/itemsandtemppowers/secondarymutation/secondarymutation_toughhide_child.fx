#############################################################
## SecondaryMutation_ToughHide_Child.fx
#############################################################

FxInfo
Lifespan 60

##############################################################

Condition
	On	Time
	Time	0

	Event
		Type Local
		At Origin
		Sound ToughHide_Child_01 30 30 .1
	End

	Event
		Type	Local
		At	Hair
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint.part
		Pother	Chest
		Lifespan 5
	End
End

Condition
	On	Time
	Time	2.5

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint_Point.part
		Lifespan 5
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint_Point.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint.part
		Part	:Chest_Ring.part
		POther	Hips
		Lifespan 5
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint.part
		Part	:Chest_Ring.part

		POther	LLegR
		Lifespan 5
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint.part
		POther	LLegL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint_Point.part
		Lifespan 5
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint_Point.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint.part
		POther	FootL
		Lifespan 5
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Body_Glow_Faint.part
		POther	FootR
		Lifespan 5
	End
End

#############################################################

End