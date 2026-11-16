#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################


Condition
	On		Time
	Time		0
	Random		1

	Event
		Type	PositOnly
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Sparks.part
		Part	:Fire_InitialFlash.part
		Part	:Fire_Anim.part
		Part	:Lava_Splash_Anim_Additive.part
		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Fire_Anim2.part
		Lifespan 	3
	End
End


#############################################################

End