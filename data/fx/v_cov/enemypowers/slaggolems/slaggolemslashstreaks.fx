#########################################################
##	Fireweapon
########################################################
FxInfo
lifespan 30 ## CL: shortened so streaks disappear right after claw swipe
######################################################################

#Condition  ##### ChrisL: not sure what this did but it appeared to be offcenter/not working properly
#	On 	Time
#	Time 	0
#
#
#	Event
#		EName	Streak1
#		Type	local
#		At	LarmR
#		Bhvr	behaviors/genericparticlefade.bhvr
#		Part1	:SlagGolemSlashBodyGlows.part
#		PMagnet	WepR
#	End
#	
#	Event
#		EName	Streak1
#		Type	local
#		At	LarmL
#		Bhvr	behaviors/genericparticlefade.bhvr
#		Part1	:SlagGolemSlashBodyGlows.part
#		PMagnet	WepL
#	End
#
#End


Condition
	On 	Time
	Time 	15


	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part3	:SlagGolemSlashGlow.part
		Sound PunchA 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	behaviors/genericparticlefade.bhvr
		Part3	:SlagGolemSlashGlow.part
	End

	
	Event
		EName	Streak1
		Type	local
		At	F2_R
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashStreak.part
	End

	Event
		EName	Streak1
		Type	local
		At	F2_L
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashStreak.part
	End

	
	Event
		EName	Streak1
		Type	local
		At	T1_R
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashStreak.part
	End

	Event
		EName	Streak1
		Type	local
		At	T1_L
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashStreak.part
	End
	
	Event
		EName	Streak1
		Type	local
		At	Rwing1
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashStreak.part
	End

	Event
		EName	Streak1
		Type	local
		At	Lwing1
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashStreak.part
	End
End


Condition
	On 	Time
	Time 	73

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

