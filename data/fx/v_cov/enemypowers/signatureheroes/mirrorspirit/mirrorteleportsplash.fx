#########################################################
##	Molten Bronze Splash
########################################################
FxInfo

############################################

Lifespan	500

############  Glob   ###################

Condition
	On 	Time
	Time 	0

	Event
		Ename	glob
		Type	Start
		At	Origin
		
	End

	Event
		Ename	blob
		Type	Local
		At	glob
		
		
		bhvr	:MoltenBronzeSplash.bhvr
		BhvrOverride
			PositionOffset		0 -.05 0
			Scale			1.0 .25 1.0
			pyrRotateJitter		180 180 180

		End

		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

		Lifespan	90
		LifespanJitter	15
	End

#	Event
#		Ename	pulse
#		Type	SetLight
#		At	blob
#		
#		bhvr	:MirrorCagePulse.bhvr
#
#		Lifespan	75
#	
#	End
#
End


##########  Heat Burst  ###############3

#Condition
#	On 	Time
#	Time 	0
#
#	Repeat	4
#
#	Event
#		Ename	Sparks
#		Type	Start
#		At	glob
#
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			PositionOffset		0 .2 0
#			InitialVelocity		.0 0.4 .0
#			InitialVelocityJitter	0.2 0.3 0.2
#		End
#
#		
#		Part	V_COV\PhysicsEnabled\MetalSpark.part
#		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#	End
#
#End
#
########  Explosion   ####################

#Condition
#	On 	Time
#	Time 	0
#
#	Repeat	3
#	RepeatJitter	1
#
#	Event
#		Ename	Chunks
#		Type	Start
#		At	glob
#
#		Geom	FX_MoltenBronzeGlob_02
#		Geom	FX_MoltenBronzeGlob_01
#
#		BhvrOverride
#			PositionOffset		0 .25 0
#			StartJitter		0 0 0
#			Scale			.4 .4 .4
#			pyrRotateJitter		180 180 180
#			SpinJitter		.5 .5 .5
#			InitialVelocity		.0 0.3 .0
#			InitialVelocityJitter	0.2 0.2 0.2
#			Drag			0.006
#			physics 		1
#			physRadius 		0.8
#			physGravity 		.9
#			physRestitution 	0.5
#			physSFriction 	 	0.3
#			physDFriction 	 	0.5
#			FadeOutLength		1110
#		End
#		CEvent 	:MirrorTeleportSplash_SM.fx
#		CThresh 0.00001
#		CDestroy	1
#
#		Lifespan	120
#		LifespanJitter	60
#	End
#
#End
#

End
