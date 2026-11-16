#############################################################
## SlowedResponse_Hit.fx
#############################################################

FxInfo
Lifespan 30

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
		Sound 	TM_SlowedResponse_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Create
		Inherit	Position
		At	T_Root
		Part	:Swirls_Explode.part
		Part	:Swirls_Explode_Flat.part
		Part	:Jaunt_Glow.part
		Part	:Jaunt_Flash_Impact.part
		Part	:Jaunt_Flash_Grow.part

		Lifespan 12
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Create
		Inherit	Position
		At	T_Root

		Part	:Spiral_Explode_Flat.part
		Part	:Spiral_Explode.part
		Lifespan 5
	End

	Event
		Type	Create
		Inherit	Position
		At	T_Root
		Bhvr	:TimeManip_Grow_Big.bhvr
		BhvrOverride
			Alpha		100
			FadeInLength	1
			FadeOutLength	40
			PYRRotate	0 90 80
			Spin 		0.0 0.0 0
			TintGeom	1
		End
		Geom	Tintable_Dark_EarthGraspHit
		Lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Create
		Inherit	Position
		At	T_Root
		Part	:Spiral_Slow_Big_Immediate.part
		Part	:Spiral_Slow_Big_Immediate.part
		Part	:Spiral_Slow_Big_Immediate_Flat.part
		Lifespan 15
	End

End



#############################################################

End