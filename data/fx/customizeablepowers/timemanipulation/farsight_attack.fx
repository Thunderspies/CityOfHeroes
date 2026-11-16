#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 100
Flags InheritGeoScale

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_Farsight_Attack_01 100 100 .8
	End
End


Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0 5.0 0.0
		End
		Part	:Portal_Swirls_Buildup.part

		Lifespan 20
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0 5.0 0.0
		End
		Part	:Portal_Swirls_Border.part
		Part	:Portal_Swirls_Border_Additive.part
		Part	:Portal_Swirls_Border_Highlights.part
		Part	:Portal_Spiral.part
		Part	:Portal_Spiral_Additive.part
		Part	:Portal_Glow.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	48

	Event
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0 5.0 0.0
		End
		Part	:Jaunt_Flash_Grow.part
		Part	:Jaunt_Glow.part
		Part	:Rays_Explode.part
		Part	:Shockwave.part
		Part	:Shockwave_Small.part
		Lifespan 30

	End
End

Condition
	On	Time
	Time	52

	Event
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0 5.0 0.0
		End
		Part	:Streaks_Explode_Short.part
		Part	:Streaks_Explode_Detached.part

		Lifespan 30

	End
End

######################################################

End