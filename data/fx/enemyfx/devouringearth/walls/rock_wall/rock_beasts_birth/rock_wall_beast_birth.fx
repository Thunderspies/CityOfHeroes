#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	InpName	Chest
End

Input
	Inpname Hips
End

Input  
	InpName	WepR
End

Input
	Inpname WepL
End

###############################################

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Local 
		At	Hips
		Part1	:StalagtiteDustHit.part
		Part1	:StalagtiteDustHitDown.part
		Part2	:StalagtiteDustHit2.part
	End

	Event
		EName 	Mallet
		Type	Local 
		At	Chest
		Part3	:hitRocks06.part
		Part4	:BoulderBreakgrey.part
		Part5	:hitRocks06.part
		#Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound stonehit2 100 100 .85

	End

End


End
