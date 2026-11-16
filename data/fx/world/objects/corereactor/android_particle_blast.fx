#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################


#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	TargetOffset
		Type	Start
		At	Target
	End

	Event
		EName 	Aim
		Type	Start
		At	Head
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


	Event
		Ename	Step2
		Type	Local
		At	Anchor
		AltPiv	2
		BhvrOverride
			StartJitter	0.2 0.4 0.2
		End
	End

	Event
		Ename	Step4
		Type	Local
		At	Anchor
		AltPiv	4
		BhvrOverride
			StartJitter	0.2 0.4 0.2
		End
	End
End

###############################################################################

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Head
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Quick_Small.part
		POther 	Step2
	End

	Event
		Type	Local
		At	Step2
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Quick_Small.part
		POther 	Step4
	End

	Event
		Type	Start
		At	Step4
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Quick_Small.part
		POther 	TargetOffset
	End
End

#############################################################

End