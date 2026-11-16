#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo
LifeSpan	60

#############################################################################

Input
	Inpname	Hips
End

#############################################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound Particle_Stunner 200 200 .8
	End

#############################################################################

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

	Event
		EName	TargetOffset
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
			StartJitter	0.1 0.1 0.1
		End

	End

	Event
		Ename	Step3
		Type	Local
		At	Anchor
		AltPiv	3

		BhvrOverride
			StartJitter	.75 .75 .75
		End

	End

	Event
		Ename	Step4
		Type	Local
		At	Anchor
		AltPiv	4

		BhvrOverride
			StartJitter	.75 .75 .75
		End

	End

	Event
		Ename	Step5
		Type	Local
		At	Anchor
		AltPiv	5

		BhvrOverride
			StartJitter	.4 .4 .4
		End

	End
End

###############################################################################

Condition
	On 	Time
	Time 	5



	Event
		Type	Local
		At	C_Hair #Step1
		Part	:Beam_Electricity_Stun.part
		Part	:Beam_Electricity_Stun_Small.part
		Part	:Beam_Electricity_Stun_Small_Inverted.part
		POther 	Step2
		Lifespan	20
	End


	Event
		Type	Local
		At	Step2
		Part	:Beam_Electricity_Stun.part
		Part	:Beam_Electricity_Stun_Small.part
		Part	:Beam_Electricity_Stun_Small_Inverted.part
		POther 	Step3
		Lifespan	20
	End


	Event
		Type	Local
		At	Step3
		Part	:Beam_Electricity_Stun.part
		Part	:Beam_Electricity_Stun_Small.part
		Part	:Beam_Electricity_Stun_Small_Inverted.part
		POther Step4
		Lifespan	20
	End

	Event
		Type	Start
		At	Step4
		Part	:Beam_Electricity_Stun.part
		Part	:Beam_Electricity_Stun_Small.part
		Part	:Beam_Electricity_Stun_Small_Inverted.part

		POther 	Step5
		Lifespan	20
	End

	Event
		Type	Local
		At	Step5
		Part	:Beam_Electricity_Stun.part
		Part	:Beam_Electricity_Stun_Small.part
		Part	:Beam_Electricity_Stun_Small_Inverted.part
		POther 	TargetOffset
		Lifespan	20
	End
End

#############################################################################

End