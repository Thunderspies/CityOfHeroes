#############################################################
## HEADER
#############################################################

FxInfo

FLAGS	InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound nictuscrystal_loop 80 80 .91
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	coreD
		Type	Local
		At	Chest
		Bhvr	:Lightform.bhvr
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	Event
		EName	core
		Type	Local
		At	coreD
		#Bhvr	:LightformA.bhvr
		Part	:LightTendrils.part
		Part	:LightTendrilsBlue.part
		Part	:LightTendrilsPurple.part
		Part	:Sparks.part
	End

	Event
		EName	spinpiv
		Type	Local
		At	core
		#Bhvr	:LightformSpin.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		part	:LightformCoreBlue.part
		part	:LightformEnergyBlue.part
		part	:LightformCorePurple.part
		part	:LightformEnergyPurple.part
	End

	Event
		EName	core2
		Type	Local
		At	Hips
		Bhvr	:Lightform2.bhvr
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	Event
		Type	Local
		At	core2
		Bhvr	:LightformSpinUp.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End

	Event
		EName	core2b
		Type	Local
		At	Hips
		Bhvr	:Lightform2.bhvr
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	Event
		Type	Local
		At	core2b
		Bhvr	:LightformSpinUp.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

Condition

	Event
		EName	coreopposite
		Type	Local
		At	Hips
		Bhvr	:Lightform1.bhvr
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	Event
		Type	Local
		At	coreopposite
		Bhvr	:LightformSpin1.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End

	Event
		EName	coreoppositea
		Type	Local
		At	Hips
		Bhvr	:Lightform1.bhvr
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	Event
		Type	Local
		At	coreoppositea
		Bhvr	:LightformSpin1.bhvr
		part	:LightformCore.part
	End

	Event
		EName	coreopposite2
		Type	Local
		At	Hips
		Bhvr	:Lightform3.bhvr
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	Event
		Type	Local
		At	coreopposite2
		Bhvr	:LightformSpindwn.bhvr
		part	:LightformCore.part
	End

	Event
		EName	coreopposite2a
		Type	Local
		At	Hips
		Bhvr	:Lightform3.bhvr
	End
End

#############################################################

End