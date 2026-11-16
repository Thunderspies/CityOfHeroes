#########################################################
##	forceBubble
########################################################
FxInfo


#################################################################################



Condition
	On 	Time
	Time 	0


	Event
		EName	Locator
		Type	start
		At	Root
		BhvrOverride
			PositionOffset		0 -11 0
		End
		
	End

#########  Beam 01   ##############

	Event
		EName	laserEye1
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam1
		Type	Local
		At	laserEye1
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:MagicLight.part
					
	End

#########  Beam 02   ##############

	Event
		EName	laserEye2
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam2
		Type	Local
		At	laserEye2
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:MagicLight.part
					
	End

#########  Beam 03   ##############

	Event
		EName	laserEye3
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam3
		Type	Local
		At	laserEye3
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:MagicLight.part
					
	End

#########  Beam 04   ##############

	Event
		EName	laserEye4
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam4
		Type	Local
		At	laserEye4
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:MagicLight.part
					
	End

#########  Beam 05   ##############

	Event
		EName	laserEye5
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam5
		Type	Local
		At	laserEye5
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			pyrRotateJitter	60 120 0
		End
		Part	:MagicLight.part
					
	End

#########  Beam 06   ##############

	Event
		EName	laserEye6
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam6
		Type	Local
		At	laserEye6
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			pyrRotateJitter	30 120 0
		End
		Part	:MagicLight.part
					
	End

#########  Beam 07   ##############

	Event
		EName	laserEye7
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		
	End

	Event
		EName	laserBeam7
		Type	Local
		At	laserEye7
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			pyrRotateJitter	12 120 0
		End
		Part	:MagicLight.part
					
	End

End