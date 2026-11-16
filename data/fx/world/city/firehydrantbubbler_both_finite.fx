#############################################################
## Geyser_Hydrant.fx
#############################################################

FxInfo
Lifespan 1800 #180

#############################################################

Input
	InpName	root
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root

		Part	:Top_Spray_Geyser.part
		Part	:Geyser_Droplets_Top.part
		Part	:Geyser_Core.part
		Part	:Geyser_Core_Highlight.part
		Sound FireHydrant_Blow1 200 200 .35

		Lifespan	1760
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
		End
		Part	:Geyser_Droplets.part
		Sound metalexplo1 200 200 .75
		Lifespan	1780

	End


	Event
		Type	Start
		At	Root
		Part	:Geyser_Ground_Splash.part
		Part	:Geyser_Ground_Splash_Alpha.part

		Part	:Geyser_Ground_Ripples.part
		Part	:Geyser_Ground_Ripples_Highlight.part
		Part	:Geyser_Ground_Mist.part

		Lifespan	1800
	End

#############################################################


	Event
		EName	Hydrant
		Type	start
		At	Root
		Geom	Praet_Street_Details_Hydrant
		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 0 0 #0 2 0

			StartJitter		0 0 0

			InitialVelocity		0 1.5 0
			InitialVelocityJitter	0.15 0.25 0.15
			Spin			0.0 0.0 0.0
			SpinJitter		0.02 0.02 0.02
			Gravity			0.04

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		60

			PhysDebris		0
			PhysDensity		2
		End
		CThresh 0.001
		CDestroy 	0
		Lifespan	9000
	End
End

Condition
	On Time
	Time 5

	Event
		Type Local
		At Root
		Sound FireHydrant_Flow1_Loop 100 100 .5
		Lifespan 	1970
	End
End

#############################################################

End