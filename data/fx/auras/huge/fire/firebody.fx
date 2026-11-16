#############################################################
## FireBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	Head
		part	:Fiery01.part
		Bhvr	:HeadOffset1.bhvr
		Pmagnet Head
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet UarmR
		POther  UarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	WepR
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	UarmR
		part	:Fiery01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:Fiery01.part
		Pmagnet UarmL
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UlegL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		Type	Local
		At	UlegR
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		part	:Fiery01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:Fiery01.part
		Pmagnet FootL
	End
End

#############################################################

End