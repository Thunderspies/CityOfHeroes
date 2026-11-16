#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	TargetOffset
		Type	Start
		At	Target
	End

	Event
		EName 	Aim
		Type	Start
		At	C_Hair
		Bhvr	behaviors/LightningBeam.bhvr
		Magnet	TargetOffset
		LookAt	TargetOffset
	End

	Event
		Ename	Anchor
		Type	Local
		At	Aim
		BhvrOverride
			Alpha		0
			PyrRotate	0 0 -90
			PyrRotateJitter	0 0 120
		End
		geom	FX_ElectricBeam_Arc

	End

End

Condition

	On	Time
	Time	1

	Event
		Ename	Step2
		Type	Local
		At	Anchor
		AltPiv	2
		BhvrOverride
			StartJitter	0.4 0.8 0.4
		End

	End

	Event
		Ename	Step3
		Type	Local
		At	Anchor
		AltPiv	3

		BhvrOverride
			StartJitter	0.8 0.8 0.8
		End

	End

	Event
		Ename	Step4
		Type	Local
		At	Anchor
		AltPiv	4

		BhvrOverride
			StartJitter	0.8 0.8 0.8
		End

	End

	Event
		Ename	Step5
		Type	Local
		At	Anchor
		AltPiv	5

		BhvrOverride
			StartJitter	0.8 0.8 0.8
		End

	End
End

###############################################################################
Condition
	On 	Time
	Time 	5
###############################################################################


	Event
		Ename	walker1
		Type	Local
		At	C_Hair
	End

	Event
		Type	Local
		At	Walker1 #Step1
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick_Small.part
		Part	:Beam_Electricity_Scroll_Small.part

		POther 	Step2
	End

###############################################################################

	Event
		Ename	walker2
		Type	Local
		At	Step2
	End

	Event
		Type	Local
		At	Walker2
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part

		POther 	Step3
	End
End
###############################################################################

Condition
	On	Time
	Time	6

	Event
		Ename	Walker3
		Type	Local
		At	Step3
	End

	Event
		Type	Local
		At	Walker3
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part

		POther Step4

	End

	Event
		Ename	Walker4
		Type	Local
		At	Step4
	End

	Event
		Type	Start
		At	Walker4
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part

		POther 	Step5

	End
End

Condition
	On	Time
	Time	7

	Event
		Ename	Walker5
		Type	Local
		At	Step5
	End

	Event
		Type	Local
		At	Walker5
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part

		POther 	TargetOffset
	End
End

#############################################################################

End