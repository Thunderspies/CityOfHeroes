#############################################################
## ObsidionArmor.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	20

	Event
		Type 	Local
		At	FootR
		bhvr	behaviors/soundFade3.bhvr
		Sound miasma_loop 60 60 .6
		Flags	PowerLoopingSound
		Lifespan 440
	End
End


Condition
	On	Time
	Time	20

	Event
		EName	2
		Type	Local
		At	FootR
		Part	:LiteDarkBlastHitTendril.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		bhvr	behaviors/soundFade3.bhvr
		Part	:LiteDarkBlastHitTendril.part
		Sound obsidian_loop 60 60 .42
		Flags	PowerLoopingSound
		Lifespan 280

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:LiteDarkBlastHitTendril.part
	End

	Event
		EName	5
		Type	Local
		At	FootL
		Part	:LiteDarkBlastHitTendril.part
		Sound rage 100 100 1.0
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	:LiteDarkBlastHitTendril.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:LiteDarkBlastHitTendril.part
	End


#############################################################
##Head
#############################################################
	Event
		EName	9
		Type	Local
		At	Head
		Part	:LiterDarkBlastHitTendril.part
	End


#############################################################
##Arms
#############################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:LiterDarkBlastHitTendril.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:LiterDarkBlastHitTendril.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:LiterDarkBlastHitTendril.part
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:LiterDarkBlastHitTendril.part
	End

End


End

