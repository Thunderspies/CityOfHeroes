#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

Lifespan 250


Condition
	On 	Time
	Time 	13

	Event
		EName 	Prime8
		Type	Local 
		At	WepR
		#Geom	FX_IceWeapon__Ice
		Geom	GEO_WepR_Sword_07
	End
	Event
		EName 	Next
		Type	Local
		At	Prime8
		AltPiv 1
		POther	Prime
	End
	Event
		EName 	Prime
		Type	Local 
		At	Prime8
		AltPiv 2
		Part5	:RapierGlow01.part
		POther	Next
		#PMagnet	Next
		
	End
End	
Condition
	On 	Time
	Time 	50

	Event	
		EName 	glowhand
		Type	Destroy


	End

End


End
