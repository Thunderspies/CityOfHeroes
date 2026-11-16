#############################################################
## MindControlHit.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:MindGlowHit2.part
		Sound mentalhit 80 80 .8
	End


End


Condition
	On 	Time
	Time	100

	Event
		Ename 	all
		Type	Destroy
	End

End



End