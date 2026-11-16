#########################################################
##	Fireweapon
########################################################
FxInfo


Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	Col_R
		Bhvr	behaviors/genericparticlefade.bhvr
		#Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	UarmR
	End
	
	Event
		EName	Streak1
		Type	local
		At	Col_R
		Bhvr	behaviors/genericparticlefade.bhvr
		#Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	UarmL
	End

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	LarmR
	End
	
	Event
		EName	Streak1
		Type	local
		At	UARML
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	WepR
		Sound PunchAA 100.0 100.0 .8
	End
	
	Event
		EName	Streak1
		Type	local
		At	LarmL
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	WepL
	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemMeleeStreak.part
		#Part2	:StrengthPulse.part
		Part3	:SlagGolemSlashGlow.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemMeleeStreak.part
		#Part2	:StrengthPulse.part
		Part3	:SlagGolemSlashGlow.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	streakUp
		Type	Destroy
		
	End
	

End


Condition
	On 	Time
	Time 	40

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

