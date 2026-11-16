#########################################################
##	Molten Bronze Splash
########################################################
FxInfo

Lifespan	500
############################################



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
			Scale			1.25 .25 1.25
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeGlob_02

		Lifespan	105
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
#	RepeatJitter	1
#
#	Event
#		Ename	Sparks
#		Type	Start
#		At	glob
#
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			PositionOffset		0 .2 0
#			InitialVelocity		.0 0.3 .0
#			InitialVelocityJitter	0.15 0.2 0.15
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
End
