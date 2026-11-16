#############################################################
## NeuronBomb_Explosion.fx
#############################################################

FxInfo
Lifespan 190

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Shake          0.4
			ShakeFallOff   .005
			ShakeRadius    70.0
		End
		Part	:Filter_Teal.part
		# Sound NeutronBomb_Explode 100 100 .8
		Lifespan 1
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Clouds_Teal.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Light_Bomb.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	45

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Shake          2.0
			ShakeFallOff   .1
			ShakeRadius    80.0
		End
		Part	:Flash_Bomb.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:White_Bomb.part
		Lifespan 1
	End
End

#Condition
#	On	Time
#	Time	160
#
#	Event
#		Type	Start
#		CameraSpace
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		Part	:Flash_Bomb_Shrink.part
#		Lifespan 1
#	End
#End
#
#Condition
#	On	Time
#	Time	180
#
#	Event
#		Type	Start
#		CameraSpace
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		Part	:Filter_Teal.part
#		Lifespan 1
#	End
#End


#############################################################

End
