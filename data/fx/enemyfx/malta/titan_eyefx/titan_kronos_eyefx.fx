#########################################################
##	
########################################################
FxInfo

Flags InheritAlpha

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	Chest
		Sound robotbeep2_loop 170 170 .28
	End
End



Condition
	Event
		EName 	Prime
		Type	Local 
		At	Chest
		Geom	Kronos_Eye_Pivot
	End

	Event
		EName 	Cylon1
		Type	Local 
		At	Prime
		  AltPiv  2
		#Part	:Titan_Eye_Glow_01.part
		Part	:Titan_Eye_Glow_03.part
	End
End

Condition
	On	Time
	Time	25

	Event
		EName 	Cylon2
		Type	Local 
		At	Prime
		  AltPiv  3
		#Part	:Titan_Eye_Glow_02.part
		Part	:Titan_Eye_Glow_04.part
	End
End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event
		EName 	All
		Type	Destroy 
	End
End


End
##################################

