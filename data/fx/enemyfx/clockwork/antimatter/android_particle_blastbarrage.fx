#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound Particle_Blast_Barrage 200 200 .8
	End

	Event
		Ename	MuzzleEffects
		Type	Local
		At	C_Hair
		Part	:Muzzle_ElectricSmoke.part
		Part	:Muzzle_Strobe_Core.part
		Part	:Muzzle_Strobe_Glow_Blue.part
		Part	:Muzzle_Electricity.part
		Lifespan 20
	End
End

#############################################################################

Condition
	On	Time
	Time	11

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
			PyrRotateJitter	0 0 45
		End
		geom	FX_ElectricBeam_Arc

	End

	Event
		Ename	AnchorB
		Type	Local
		At	Aim
		BhvrOverride
			Alpha		0
			PyrRotate	0 0 -210
			PyrRotateJitter	0 0 45
		End
		geom	FX_ElectricBeam_Arc

	End

	Event
		Ename	AnchorC
		Type	Local
		At	Aim
		BhvrOverride
			Alpha		0
			PyrRotate	0 0 -330
			PyrRotateJitter	0 0 45
		End
		geom	FX_ElectricBeam_Arc

	End
End

###############################################################################

Condition

	On	Time
	Time	12

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
			StartJitter	1.0 1.0 1.0
		End

	End

	Event
		Ename	Step4
		Type	Local
		At	Anchor
		AltPiv	4

		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End

	End

	Event
		Ename	Step5
		Type	Local
		At	Anchor
		AltPiv	5

		BhvrOverride
			StartJitter	0.8 0.4 0.8
		End

	End

###############################################################################


	Event
		Ename	Step2B
		Type	Local
		At	AnchorB
		AltPiv	2
		BhvrOverride
			StartJitter	0.4 0.8 0.4
		End

	End

	Event
		Ename	Step3B
		Type	Local
		At	AnchorB
		AltPiv	3

		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End

	End

	Event
		Ename	Step4B
		Type	Local
		At	AnchorB
		AltPiv	4

		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End

	End

	Event
		Ename	Step5B
		Type	Local
		At	AnchorB
		AltPiv	5

		BhvrOverride
			StartJitter	0.8 0.4 0.8
		End

	End

###############################################################################

	Event
		Ename	Step2C
		Type	Local
		At	AnchorC
		AltPiv	2
		BhvrOverride
			StartJitter	0.4 0.8 0.4
		End

	End

	Event
		Ename	Step3C
		Type	Local
		At	AnchorC
		AltPiv	3

		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End

	End

	Event
		Ename	Step4C
		Type	Local
		At	AnchorC
		AltPiv	4

		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
	End

	Event
		Ename	Step5C
		Type	Local
		At	AnchorC
		AltPiv	5

		BhvrOverride
			StartJitter	0.8 0.4 0.8
		End

	End
End
###############################################################################

Condition
	On 	Time
	Time 	16


	Event
		Type	Local
		At	C_Hair #Step1
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick_Small.part
		Part	:Beam_Electricity_Scroll_Small.part
		POther 	Step2
	End



	Event
		Type	Local
		At	Step2
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	Step3
	End
End

Condition
	On	Time
	Time	17


	Event
		Type	Local
		At	Step3
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther	Step4
	End



	Event
		Type	Start
		At	Step4
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	Step5
	End
End

Condition
	On	Time
	Time	18


	Event
		Type	Local
		At	Step5
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	TargetOffset
	End
End

###############################################################################

Condition
	On 	Time
	Time 	19


	Event
		Type	Local
		At	C_Hair #Step1
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick_Small.part
		Part	:Beam_Electricity_Scroll_Small.part
		POther 	Step2B
	End



	Event
		Type	Local
		At	Step2B
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	Step3B
	End
End

Condition
	On	Time
	Time	20


	Event
		Type	Local
		At	Step3B
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther Step4B
	End



	Event
		Type	Start
		At	Step4B
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	Step5B
	End
End

Condition
	On	Time
	Time	21


	Event
		Type	Local
		At	Step5B
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	TargetOffset
	End
End
###############################################################################

Condition
	On 	Time
	Time 	22


	Event
		Type	Local
		At	C_Hair #Step1
		Part	:Beam_Electricity_Linger_Small.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick_Small.part
		Part	:Beam_Electricity_Scroll_Small.part
		POther 	Step2C
	End



	Event
		Type	Local
		At	Step2C
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	Step3C
	End
End

Condition
	On	Time
	Time	23


	Event
		Type	Local
		At	Step3C
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther Step4C
	End



	Event
		Type	Start
		At	Step4C
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	Step5C
	End
End

Condition
	On	Time
	Time	24


	Event
		Type	Local
		At	Step5C
		Part	:Beam_Electricity_Linger.part
		Part	:Beam_Electricity_Linger_Inverted.part
		Part	:Beam_Electricity_Quick.part
		Part	:Beam_Electricity_Scroll.part
		POther 	TargetOffset
	End
End
###############################################################################

End