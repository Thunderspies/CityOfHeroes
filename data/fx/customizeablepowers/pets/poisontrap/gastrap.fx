#############################################################
## GasTrap.fx
#############################################################

FxInfo
LifeSpan	4700

#################################################################################


Condition
	On	time
	Time	14

	Event
		Type	local
		At	root
		Sound air2 80 80 .8
	End
End


Condition
	On	time
	Time	20

	Event
		Type	local
		At	root
		Sound miasma_loop 80 80 .33
	End
End



Condition
	On 	Time
	Time 	0


	Event
		Ename	hookup
		Type	local
		At	root
		Bhvr	V_COV\Powers\Traps\PoisonOffset.bhvr
		Sound grexplo4 80 80 .9


	End

	Event
		EName	minepivot
		Type	start
		At	hookup
		Geom	gadgetHookup
	End

	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		Bhvr	V_COV\Powers\Traps\GreenGlassMineScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PoisonGlassBall
	End

#########################################################################################

	Event
		Ename	Polegear
		Type	start
		At	minepivot
		altpiv	1

		Bhvr	V_COV\Powers\Traps\PoleScale.bhvr
		BhvrOverride
			StartColor 0 0 0
		End
		Geom	CloakingPole
	End
#########################################################################################

	Event
		Type	start
		At	minepivot
		altpiv	1

		Bhvr	V_COV\Powers\Traps\Poleoffset.bhvr
		Part	:AcidTrapDrops.part
		Part	:AcidTrapDropsLt.part

	End

	Event
		Type	start
		At	minepivot
		altpiv	1

		Bhvr	V_COV\Powers\Traps\PoleoffsetHigher.bhvr

		Part	:Acidcore.part
		Part	:Acidcore2.part

		Part	:Acidsteam.part
		Part	:AcidsteamDark.part
		Part	:AcidDark.part
		Part	:AcidBubbleDark.part

		Part	:AcidLiquid.part

	End
End

#################################################################################

End