#############################################################
## DualBlades - AOE Bridge Attack
#############################################################

FxInfo

LifeSpan	180

#############################################################

Condition
	On 	Time
	Time 	14

	Event
		Type	Start
		At	Root
		Bhvr	POWERS\MedievalWeapons\DualBlades\DualBlade_AOESphereScaler1.bhvr
	#	Geom	SuperStrengthBubble
		Lifespan	2
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	Mystic
		Sound WWV_DanceOfBlades_Attack 150.0 150.0 .8
	End

	Event
		Type	Local
		At	WepR
		Part	:Blade_Swirl_Small.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan 2
	End
End

#############################################################

Condition
	On 	Time
	Time 	31

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Blade_Swirl.part
		Part	:Blade_Blur.part
		Lifespan	10
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Blade_Swirl.part
		Part	:Blade_Blur.part
		Lifespan	10
	End


End

#############################################################

End