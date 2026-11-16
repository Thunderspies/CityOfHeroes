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
		EName	BladeAnchorL
		Type	Local
		At	WepL
		Sound WWV_BladeFlurry_Attack 150.0 150.0 .8
		Lifespan 50
	End

	Event
		Type	Start
		At	Root
		Part	:Blade_Blur_Root.part
		Lifespan	12
	End

	Event
		Type	Local
		At	WepR
		Part	:Blade_Swirl.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End

	Event
		Type	Local
		At	WepL
		Part	:Blade_Swirl.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End
End

Condition
	On 	Cycle
	Time 	5

	Event
		Type	Start
		At	BladeAnchorL
		Part	:Blade_Swirl.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End

	Event
		Type	Local
		At	BladeAnchorR
		Part	:Blade_Swirl.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	83

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
		End
		Part	:Blade_Swirl.part
		Part	:Blade_Swirl_Fuzzy.part
		Part	:Blade_Blur_Small.part
		Lifespan	2
	End


	Event
		Type	Start
		At	Root
		Part	:Blade_Swirl_Fuzzy.part
		Part	:Blade_Blur_Root.part
		Lifespan	2
	End

End

#############################################################

End