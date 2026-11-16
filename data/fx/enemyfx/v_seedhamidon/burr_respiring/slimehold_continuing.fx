#########################################################
##	forceBubble
########################################################
FxInfo

Input
	Inpname	Root
End

Input
	Inpname	wepR
End

Input
	Inpname	wepL
End

Input
	Inpname	LlegR
End

Input
	Inpname	LlegL
End
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	webcage
		Type	posit
		At	root
		#Geom	netwebbingsmallCage
		Bhvr	Behaviors/GenericPArticleFade.bhvr
		BhvrOverride
			Scale 1 0.1 1
		End
	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	1
		Part	:SlimeWeb.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	1
		Part	:SlimeWeb.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	2
		Part	:SlimeWeb.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	10
		Part	:SlimeWeb.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	9
		Part	:SlimeWeb.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	3
		Part	:SlimeWeb.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	8
		Part	:SlimeWeb.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	4
		Part	:SlimeWeb.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	7
		Part	:SlimeWeb.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	5
		Part	:SlimeWeb.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	6
		Part	:SlimeWeb.part
		POther	LlegR

	End


#######################################################################


	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	1
		Part	:SlimeWeb.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	1
		Part	:SlimeWeb.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	2
		Part	:SlimeWeb.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	3
		Part	:SlimeWeb.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	4
		Part	:SlimeWeb.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	5
		Part	:SlimeWeb.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	6
		Part	:SlimeWeb.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	7
		Part	:SlimeWeb.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	8
		Part	:SlimeWeb.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	9
		Part	:SlimeWeb.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	PositOnly
		At	WebCage
		Altpiv	10
		Part	:SlimeWeb.part
		POther	WepR

	End



	Event
		EName	lazer
		Type	local
		At	root


		#Geom	web1


	End

	Event
		EName	lazer
		Type	local
		At	root


		#Geom	web2


	End

	Event
		EName	lazer
		Type	local
		At	root


		#Geom	web3


	End

	Event
		EName	lazer
		Type	local
		At	root


		#Geom	web4


	End

End


End