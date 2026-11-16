#############################################################
## IceHands.fx
#############################################################

FxInfo

Lifespan 35

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LARMR
		Bhvr	:icehands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	:ColdArmMist2.part
		geom	Tintable_IceHandR
		Sound Icepunch3 70 70 0.7
	End

	Event
		Type	Local
		At	LARML
		Bhvr	:iceHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	:ColdArmMist2.part
		geom	Tintable_IceHandL
	End

	Event
		Type	Local
		At	WepL
		Part	:Snowfall.part
		Part	:IceArmorHandBitsA.part
		Lifespan 35
	End

	Event
		Type	Local
		At	WepR
		Part	:Snowfall.part
		Part	:IceArmorHandBitsA.part
		Lifespan 35
	End

End
#############################################################
End