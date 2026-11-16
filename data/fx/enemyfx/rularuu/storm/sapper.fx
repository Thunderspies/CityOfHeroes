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
	Time 	30

	Event
		Type	Local
		At	chest
		Part	:SapperGunTipAttackGlow.Part
		Part	:SapperCoreGlow.Part
		POther Target	
		Lifespan 70
	End

	Event
		Type	Local
		At	chest
		Part	:SapperStream01.part
		Part	:SapperStream02.part
		Part	:SapperStream03.part
		Part	:SapperStream04.part
		Part	:SapperStream05.part
		Part	:SapperStream06.part
		Part	:SapperStream07.part
		POther Target	
		Lifespan 60
	End	
	
	Event
		Type	Local
		At	Target
		Part	:SapperGunTipAttackGlow.Part
		Lifespan 70
	End

	Event
		Type	Local
		At	Target
		Part	:SapperGunTipTargetGlow.Part
		Part	:SapperCoreGlow.Part
		Lifespan 72
	End
	
End


End