#############################################################
## Pocket_D_TeleportCastRecall.fx
#############################################################

FxInfo

Lifespan 636

#########################################################
#####################    Audio    ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	BuildupSound
		Type	Local
		At	Chest
		BHVR	behaviors\fadeout.bhvr
		Sound PockD_teleport2_Loop 80 80 0.8
	End
End

Condition
	On	Time
	Time	347

	Event
		ENAME	ActivationSound
		Type	Local
		At	Chest
		BHVR	behaviors\fadeout.bhvr
		Sound rumble7 80 80 0.55
	End
End

#########################################################
#####################    Cast    ########################
#########################################################

Condition
	On	Time
	Time	5

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSHeadGlow02.part
		Pmagnet HandR
		Lifespan 460
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSHeadGlow02.part
		Pmagnet HandL
	End
End

#########################################################
#####################    1st Buildup    #################
#########################################################

Condition
	On	Time
	Time	70

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSHeadGlow02.part
		Part	:TSHeadGlow01_Thin.part
		Pmagnet HandR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSHeadGlow02.part
		Part	:TSHeadGlow01_Thin.part
		Pmagnet HandL
	End
End

Condition
	On	Time
	Time	90

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSHeadGlow02.part
		Part	:TSHeadGlow01_Thin.part
		Pmagnet HandR
		Lifespan 50
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSHeadGlow02.part
		Part	:TSHeadGlow01_Thin.part
		Pmagnet HandL
		Lifespan 50
	End
End

#########################################################
#####################    2nd Buildup    #################
#########################################################

Condition
	On	Time
	Time	140

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSHeadGlow02.part
		Part	:TSHeadGlow01_Thin.part
		Part	:TSHeadGlow01_Thin.part
		Part	:TSHeadGlow02_Thin.part
		Pmagnet HandR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSHeadGlow02.part
		Part	:TSHeadGlow01_Thin.part
		Part	:TSHeadGlow01_Thin.part
		Part	:TSHeadGlow02_Thin.part
		Pmagnet HandL
	End
End

#########################################################
#####################    3rd Buildup    #################
#########################################################

Condition
	On	Time
	Time	220

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSSparks_Thin.part
		Part	:TSSparks_small.part
		Part	:TS_thin_Rays.part
		Pmagnet HandR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSSparks_Thin.part
		Part	:TSSparks_small.part
		Part	:TS_thin_Rays.part
		Pmagnet HandL
	End
End

#########################################################
#####################    4th Buildup    #################
#########################################################

Condition
	On	Time
	Time	320

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSHeadGlow_Large.part
		Part	:TSHeadGlow_Large_Thin.part
		Part	:TSSparks_smallRed.part
		Pmagnet HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSHeadGlow_Large.part
		Part	:TSHeadGlow_Large_Thin.part
		Part	:TSSparks_smallRed.part
		Pmagnet HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End
End

#########################################################
#####################    5th Buildup    #################
#########################################################

Condition
	On	Time
	Time	400

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:TSSparks_Thin.part
		Part	:TSSparks_small.part
		Part	:TS_thin_Rays.part
		Part	:TSSparks_ThinRed.part
		Pmagnet HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSSparks_Thin.part
		Part	:TSSparks_small.part
		Part	:TS_thin_Rays.part
		Part	:TSSparks_ThinRed.part
		Pmagnet HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End
End

#########################################################
#####################    RingBurst    #################
#########################################################

Condition
	On	Time
	Time	463

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:TSGlow_Ring.part
		Part	:TSGlow_Ring.part
		Part	:TSGlow_Ring.part
		Pmagnet HandL
	End
End

#############################################################

End