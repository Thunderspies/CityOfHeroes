#########################################################
##	template

FxInfo

Input  
	Inpname	Origin
End

Lighting	1	##Generate your own light


Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Origin
		Anim	Scam_01
	End
End

Condition  ### Sound will NOT play on a non-player anim without a particle in the script
	On Chance
	Chance .0
	Event
		Type Local 
		At	Hips
		Part flash.part
	End
End


Condition
	On 	Cycle
	Time 100

	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound SecurityCamera_Scan_01 25 25 .1
		Sound SecurityCamera_Scan_02 25 25 .1
		Sound SecurityCamera_Scan_03 25 25 .1
		Sound SecurityCamera_Scan_04 25 25 .1
		Sound SecurityCamera_Scan_05 25 25 .1
		Sound SecurityCamera_Scan_06 25 25 .1
		Lifespan 60
	End
End	