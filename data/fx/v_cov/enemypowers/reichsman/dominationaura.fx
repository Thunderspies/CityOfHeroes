#########################################################
##	FireAura
FxInfo

##################################


Condition
	On	Time
	Time	0

	Event
		Ename	GroundDecal
		Type	PositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
		End
		Part	:GroundDecal.part
		Part	:GroundDecalSub.part
		Part	:GroundDecalStill.part
		part	:GroundDecalSolid.part
		sound AuraOfDomination_Loop 40 20 0.5
	End
End



Condition
	On	Time
	Time	0

	Event
		Ename	Spinner3
		Type	Local
		At	Root
		BhvrOverride
			Spin 0.0 .05 0.0
			SpinJitter 0.0 .05 0.0
		End
	End

	Event
		Ename 	Dust3
		Type	Local
		At	Root
		ChildFX	:Streaks.fx
	End

	Event
		Ename 	Embers
		Type	Local
		At	Spinner3
		Part	:FlyingEmbers.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		Pmagnet Head
		BhvrOverride
			Scale 5.0 5.0 5.0
		End

		part	:TorchflameAdditive.part
		part	:TorchflameSub2.part

	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		Pmagnet Head
		BhvrOverride
			StartJitter 0.0 1.0 0.0
			Scale 6.5 6.5 6.5
		End

		part	:TorchflameAdditive.part
		part	:TorchflameHighlight.part
		part	:TorchflameSubTrail2.part


	End

	Event
		EName	Neck
		Type	Local
		At	Neck
		BhvrOverride
			Scale 1.5 1.5 1.5
		End

		part	:TorchflameAdditive.part
		part	:TorchflameHighlight.part
		part	:TorchflameSubTrail2.part
		part	:TorchflameAdditiveTrail.part

	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		Pmagnet LLegL
		BhvrOverride
			Scale 1.0 1.0 1.0
		End

		part	:TorchflameAdditiveFeet.part
		part	:TorchflameHighlightFeet.part
		part	:TorchflameSub.part
		part	:TorchflameSubTrail2.part
		part	:TorchflameAdditiveTrail.part

	End


	Event
		EName	Rleg
		Type	Local
		At	FootR
		Pmagnet LLegR
		BhvrOverride
			Scale 1.0 1.0 1.0
		End

   		part	:TorchflameAdditiveFeet.part
		part	:TorchflameHighlightFeet.part
		part	:TorchflameSub.part
		part	:TorchflameSubTrail2.part
		part	:TorchflameAdditiveTrail.part
	End
End



End