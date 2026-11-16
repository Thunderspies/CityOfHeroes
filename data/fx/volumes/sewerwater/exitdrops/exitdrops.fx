#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Lifespan 20


Condition
	On	Time
	Time	0

	Event
		EName 	Prime8
		Type	Start
		At	Origin
		Part1 	:Greensplash_Steam.part
		Part2	:Greensplash_drops2.part
		Part3   :Greensplash_Mist.part
		Part4   :Greensplash_ripple.part
	End

End
Condition
	On Time
	Time 15
	Event
		Type Local
		At Origin
		Sound WaterExit1 50 50 .35
		Sound WaterExit2 50 50 .35
		Sound WaterExit3 50 50 .35
		Sound WaterExit4 50 50 .35
		Sound WaterExit5 50 50 .35
	End
End
End
