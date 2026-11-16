#############################################################
## Child_ShieldCharge_GroundFX.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
	End

	Event
		Type	Start
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
	End

	Event
		Type	Start
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
	End

	Event
		Type	Start
		At	UArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	Neck
	End

	Event
		Type	Start
		At	LArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	UarmL
	End

	Event
		Type	Start
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	Neck
	End

	Event
		Type	Start
		At	LArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	UarmR
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
	End

	Event
		Type	Start
		At	HandL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	LarmL
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	LarmR
	End

	Event
		Type	Start
		At	LLegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	UlegR
	End

	Event
		Type	Start
		At	UlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
		#Pother	Hips
	End

	Event
		Type	Start
		At	LlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	UlegL
	End

	Event
		Type	Start
		At	FootL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	LlegL
	End

	Event
		Type	Start
		At	FootR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur2.part
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#		Pother	LlegR
	End

#	Event
#		Type	Start
#		At	FootL
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#	End
#
#	Event
#		Type	Start
#		At	FootR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Part	CustomizeablePowers\MartialManipulation\Subtractive\MotionBlur_Core.part
#	End
End

#############################################################

End