#############################################################
## Hydrant_Geyser_Force.fx
#############################################################

FxInfo
OnForceRadius 10

#############################################################

Condition
	On		Force
	ForceThreshold	10
	DoMany		1
	Time		9
	Repeat		1

	Event
		Type	Start
		At	Origin

		Part	:Top_Spray_Geyser.part
		Part	:Geyser_Droplets_Top.part
		Part	:Geyser_Core.part
		Part	:Geyser_Core_Highlight.part
		Part	:Geyser_Ground_Ripples.part
		Part	:Geyser_Ground_Ripples_Highlight.part
		Part	:Geyser_Ground_Mist.part
		Lifespan	150
	End

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
		End
		Part	:Geyser_Droplets.part
		Lifespan	150

	End


	Event
		Type	Start
		At	Root
		BhvrOVerride
			PositionOffset	0.0 -0.65 0
		End
		Part	:Geyser_Ground_Splash.part
		Lifespan	180
	End

#
#End
#
#Condition
#	On		Force
#	ForceThreshold	10

	Event
		EName	Hydrant
		Type	start
		At	Origin
		Geom	Praet_Street_Details_Hydrant
		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 2 0

			StartJitter		0 0 0
			scale			1.0 1.0 1.0

			InitialVelocity		0 1.5 0
			InitialVelocityJitter	0.15 0.25 0.15
			Spin			0.0 0.0 0.0
			SpinJitter		0.02 0.02 0.02
			StartColor		140 140 140
			Gravity			0.04

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.3
			physDFriction 	 	0.5
	#		FadeOutLength		510

			PhysDebris		1
			PhysDensity		2
		End
		CThresh 0.001
		CDestroy 	0
		Lifespan	90
	End
End

#############################################################

End