#############################################################
## DeathAndGlory_Hit.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	0 0 90
			Positionoffset	-0.6 0 0
		End
		Part	:Bracelet_Rings.part
		Lifespan	60
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	0 0 -90
			Positionoffset	0.6 0 0
		End
		Part	:Bracelet_Rings.part
		Lifespan	60
	End

	Event
		Type	Local
		At	LLegL
		BhvrOverride
			Positionoffset	0 -1.25 -0.25
		End
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Bracelet_Rings.part
		Lifespan	60
	End

	Event
		Type	Local
		At	LLegR
		BhvrOverride
			Positionoffset	0 -1.25 -0.25
		End
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Bracelet_Rings.part
		Lifespan	60
	End

	Event
		Type	Local
		At	Neck
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Bracelet_Rings.part
		Lifespan	60
	End


	Event
		Type	Local
		At	Chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Chest_Heal_Glow.part
		Lifespan	60
	End
End

#############################################################

End