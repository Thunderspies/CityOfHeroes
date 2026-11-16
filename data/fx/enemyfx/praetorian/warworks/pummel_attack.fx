#############################################################
## Smash_Activate.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 -0.2 0.0
		End
		Part	:Hands_Glow_Blue.part
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	1.5 -0.2 0.0
		End
		Part	:Hands_Glow_Blue.part
		Lifespan	30
		LifespanJitter	5
	End

End

Condition
	On	Time
	Time	15
	
	Event
		Type	Local
		At	Root
		Sound WW_Pummel_Attack 200 200 .8
	End
End


Condition
	On	Time
	Time	25
		
	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 -0.2 0.0
		End
		Part	:Hands_Rings_Blue.part
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	1.5 -0.2 0.0
		End
		Part	:Hands_Rings_Blue.part
		Lifespan	30
		LifespanJitter	5
	End
End

Condition
	On	Time
	Time	52

	Event
		Type	PositOnly
		At	WepL
		Part	:Gust_Streaks.part
		Part	:Gust_Streaks2.part

		Lifespan 10
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Gust_Streaks.part
		Part	:Gust_Streaks2.part

		Lifespan 10
	End
End

Condition
	On	Time
	Time	54

	Event
		Type	PositOnly
		At	WepL
		Part	:Gust_Rings.part
		Lifespan 7
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Gust_Rings.part
		Lifespan 7
	End
End

Condition
	On	Time
	Time	56

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-1.5 0 0
			PyrRotate	0 90 0
			Scale		1.0 1.0 1.0
			StartColor	75 50 255
		End
		Geom	Tintable_ForceRepulsion01
		Lifespan 6
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	1.5 0 0
			PyrRotate	0 -90 0
			Scale		1.0 1.0 1.0
			StartColor	100 50 255

		End
		Geom	Tintable_ForceRepulsion01
		Lifespan 6
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	PositOnly
		At	WepL
		Part	:Hit_Shockwave.part
		Part	:Hit_Flash_Large.part
		Part	:Hit_Flash_Small_Yellow.part
		Lifespan 7
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Hit_Shockwave.part
		Part	:Hit_Flash_Large.part
		Part	:Hit_Flash_Small_Yellow.part
		Lifespan 7
	End
End

#############################################################

End
