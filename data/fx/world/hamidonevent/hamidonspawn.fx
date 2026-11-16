#########################################################
##	MH
## 	Female Right Foot

FxInfo

Input
	InpName Origin
End


Condition
	Event
		Type Local
		At origin
		Sound hamidonspwn 18000 18000 .98
	End
End

Condition
	On 	Time
	Time 	200
	
	Event
		Type Local
		At origin
		Sound thepit_loop 18000 18000 .68
	End
End

End