#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################


Condition
	On	Time
	Time	23

	Event
		EName	SplashAnchor
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Splash_Droplets_Small.part
		Part	:Toxic_Splash_Small.part
	End
End

Condition
	On	Time
	Time	23
	Chance	1.0

	Event
		Type	PositOnly
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Strands.part
	End
End

Condition
	On	Time
	Time	28
	DoMany 	1

	Event
		Type	Start
		At	Root
		Until	FLY
		ChildFX	:CHILD_Splatter_SmallR.fx
		Lifespan 1
	End
End


#############################################################

End