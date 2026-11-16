#############################################################
## SelfDestruction_Windup.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound RobotSelfDestructWindup_01 60 60 .975
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	Root
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake          0.055	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0001	#how long shake lasts
			ShakeRadius    64	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
		Lifespan 5
	End
End

#Condition
#	On 	Time
#	Time 	140
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	Behaviors\CameraShake01.bhvr
#		BhvrOverride
#			Shake          0.055	#How much to shake the camera when event with this bhvr is created
#			ShakeFallOff   -0.0015	#how long shake lasts
#			ShakeRadius    96.0	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
#		End
#		Lifespan 5
#	End
#End
#
#Condition
#	On 	Time
#	Time 	290
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	Behaviors\CameraShake01.bhvr
#		BhvrOverride
#			Shake          0.375	#How much to shake the camera when event with this bhvr is created
#			ShakeFallOff   0.025	#how long shake lasts
#			ShakeRadius    0.0	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
#		End
#		Lifespan 10
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	Root
		ChildFX	:Child_ElectricalBodySurge1.fx
		Lifespan 290
	End
End

Condition
	On 	Time
	Time 	60

	Event
		Type	Local
		At	Root
		ChildFX	:Child_RandomBodySurge1.fx
		Lifespan 230
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Hips
		ChildFX	:Child_RandomSparks1.fx
		Lifespan 265
	End
End

Condition
	On 	Time
	Time 	165

	Event
		Type	Local
		At	Hips
		ChildFX	:Child_RandomSparks2.fx
		Lifespan 130
	End
End
#############################################################

Condition
	On 	Time
	Time 	60

	Event
		Type	Local
		At	Root
		ChildFX	:Child_RandomSteamJets1.fx
		Lifespan 135
	End
End

Condition
	On 	Time
	Time 	165

	Event
		Type	Local
		At	Root
		ChildFX	:Child_ConstantSteamJets1.fx
		Lifespan 130
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		ChildFx	:Child_CountdownFlash_Slow.fx
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	105

	Event
		Type	Local
		At	Hips
		ChildFx	:Child_CountdownFlash_Medium.fx
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	195

	Event
		Type	Local
		At	Hips
		ChildFx	:Child_CountdownFlash_Fast.fx
		Lifespan 105
	End
End

#############################################################

End