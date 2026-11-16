#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 100

#############################################################



Condition
	On	Time
	Time	22

	Event
		Ename	BeamAnchor
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0 -0.25 -1
		End
		Bhvr	:Tracer_Fade.bhvr
	End

	Event
		Ename	BeamEnd
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-5 11 45
		End
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.25 0.3 2
		End
		Part	:MuzzleFlash_Continuous.part
		Part	:Muzzle_Glow_Core_Continuous.part
		Part	:Muzzle_Flash_Glow.part
		Lifespan 3
	End

	Event
		Ename	AfterGlowAnchor
		Type	Local
		At	BeamAnchor
		Lifespan 13
	End
End



####################

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.25 0.3 2
		End
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 20
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.25 0.3 1.2
			PyrRotate		10 -10 0
		End
		Part	:MuzzleFlash_Gas03.part
		PMagnet	BeamEnd
		Lifespan 20
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	BeamAnchor
		#Part	:PreciseBeam03.part
		Part	:Tracer_Trail05.part
		Part	:Beam_Small_Continuous.part
		Part	:Beam_Small_Continuous_Add.part
		Part	:Beam_Flare03.part
		Part	:Beam_Small_Continuous_Electricity.part
		Part	:Beam_Small_Continuous_Electricity_Blur.part
		LookAt	BeamEnd
		Pother	BeamEnd
		PMagnet	BeamEnd
		Lifespan 20
	End

	Event
		Type	Local
		At	BeamAnhor
		BhvrOverride
			PositionOffset	0 0 -1
		End
	End
End


Condition
	On	Cycle
	Time	1

	Event
		Type	Start
		At	AfterGlowAnchor
		Part	:Beam_Small_AfterGlow.part
		#Part	:Beam_Small_AfterGlow_Electricity.part
		PMagnet	BeamEnd
		LookAt	BeamEnd
		Pother	BeamEnd
		Lifespan 10
	End
End

#############################################################

End