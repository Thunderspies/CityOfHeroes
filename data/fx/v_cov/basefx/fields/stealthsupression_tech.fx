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
			PositionOffset 0 -1 0
		End
		
	End


#########  Beam 01   ##############

	Event
		EName	laserEye1
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam1
		Type	Local
		At	laserEye1
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:LazerLight.part
					
	End

#########  Beam 02   ##############

	Event
		EName	laserEye2
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam2
		Type	Local
		At	laserEye2
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:LazerLight.part
					
	End

#########  Beam 03   ##############

	Event
		EName	laserEye3
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam3
		Type	Local
		At	laserEye3
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:LazerLight.part
					
	End

#########  Beam 04   ##############

	Event
		EName	laserEye4
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam4
		Type	Local
		At	laserEye4
		Bhvr	:LightAngle.bhvr
		BhvrOverride
		End
		Part	:LazerLight.part
					
	End

#########  Beam 05   ##############

	Event
		EName	laserEye5
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam5
		Type	Local
		At	laserEye5
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			pyrRotateJitter	60 120 0
		End
		Part	:LazerLight.part
					
	End

#########  Beam 06   ##############

	Event
		EName	laserEye6
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam6
		Type	Local
		At	laserEye6
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			pyrRotateJitter	30 120 0
		End
		Part	:LazerLight.part
					
	End

#########  Beam 07   ##############

	Event
		EName	laserEye7
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam7
		Type	Local
		At	laserEye7
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			pyrRotateJitter	12 120 0
		End
		Part	:LazerLight.part
					
	End

#########  Beam 08   ##############

	Event
		EName	laserEye8
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam8
		Type	Local
		At	laserEye8
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			#pyrRotate	60 0 0
			pyrRotateJitter	6 120 0
		End
		Part	:LazerLight.part
					
	End

#########  Beam 09   ##############

	Event
		EName	laserEye9
		Type	start
		At	Locator
		Bhvr	:LightSpin.bhvr
		BhvrOverride
		End
		
	End

	Event
		EName	laserBeam9
		Type	Local
		At	laserEye9
		Bhvr	:LightAngle.bhvr
		BhvrOverride
			#pyrRotate	36 0 0
			pyrRotateJitter	6 120 0
		End
		Part	:LazerLight.part
					
	End

End