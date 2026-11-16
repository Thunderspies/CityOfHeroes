#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 120

#############################################################
## LOOP
#############################################################

Condition
	On 	Time
	Time 	22


	Event
		Type	Local
		At	Chest
		POther	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Chest.part
		Lifespan 7
	End

	Event
		Type	Local
		At	Chest
		POther	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Chest.part

		Lifespan 7
	End
End

Condition
	On	Time
	Time	35


	Event
		Type	Local
		At	UArmL
		POther	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Arms.part
		Lifespan 10
	End

	Event
		Type	Local
		At	UArmR
		POther	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Arms.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	45


	Event
		Type	Local
		At	LArmL
		POther	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Limbs.part
		Lifespan 7
	End

	Event
		Type	Local
		At	LArmR
		POther	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Limbs.part
		Lifespan 7
	End

	Event
		Type	Local
		At	Hips
		POther	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Limbs.part
		Part	:Tumors_Limbs_Growing.part

		Lifespan 7
	End

	Event
		Type	Local
		At	Hips
		POther	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Limbs.part
		Part	:Tumors_Limbs_Growing.part

		Lifespan 7
	End

	Event
		Type	Local
		At	LLegL
		POther	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Limbs.part
		Part	:Tumors_Limbs_Growing.part

		Lifespan 7
	End

	Event
		Type	Local
		At	LLegR
		POther	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Limbs.part
		Part	:Tumors_Limbs_Growing.part

		Lifespan 7
	End
End

Condition
	On	Time
	Time	60

		Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Chest_Growing.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	63

	Event
		Type	Local
		At	ULegR
		POther	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Chest_Growing.part
		Lifespan 7
	End

	Event
		Type	Local
		At	ULegL
		POther	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tumors_Chest_Growing.part
		Lifespan 7
	End
End

#############################################################

End