#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

Input  
	Inpname	HandL
End

Lifespan 200


########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	GunTip
		Type	local 
		At	WepR
		Bhvr	:SapperGunTipOffset.bhvr
		Lifespan 30
	End

	Event
		Type	Local
		At	GunTip
		Part	:SapperGunTipAttackGlow.Part
		POther Target	
		Lifespan 30
	End
	
End

Condition
	On 	Time
	Time 	20

	
	Event
		Type	Local
		At	Target
		Part	:SapperGunTipAttackGlow.Part
		Sound sapper3 60 60 .55
		Lifespan 10
	End

	Event
		Type	Local
		At	Target
		Part	:SapperGunTipTargetGlow.Part
		Lifespan 12
	End
	
End


End