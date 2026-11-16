#############################################################
## webCage.fx
#############################################################

FxInfo

#################################################################################

Input
	Inpname	Hips
End

#################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	root
		Sound webgrexplo2 88 88 .95
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName	webcage
		Type	posit
		At	root
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_netwebbingsmallCage

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	:web.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	:web.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	2
		Part	:web.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	10
		Part	:web.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	9
		Part	:web.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	3
		Part	:web.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	8
		Part	:web.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	4
		Part	:web.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	7
		Part	:web.part
		POther	LlegR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	5
		Part	:web.part
		POther	LlegL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	6
		Part	:web.part
		POther	LlegR

	End


#######################################################################


	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	:web.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	:web.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	2
		Part	:web.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	3
		Part	:web.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	4
		Part	:web.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	5
		Part	:web.part
		POther	WepL

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	6
		Part	:web.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	7
		Part	:web.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	8
		Part	:web.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	9
		Part	:web.part
		POther	WepR

	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	10
		Part	:web.part
		POther	WepR

	End



	Event
		EName	lazer
		Type	local
		At	root
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_web1


	End

	Event
		EName	lazer
		Type	local
		At	root
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_web2


	End

	Event
		EName	lazer
		Type	local
		At	root
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_web3


	End

	Event
		EName	lazer
		Type	local
		At	root
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_web4


	End

End


End