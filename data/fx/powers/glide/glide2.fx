#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	Chest
End

Input  
	InpName	Hips
End

##################################
Condition
	On	Time
	Time	0
		
	Event
		Type	Local 
		At	Root
		part	:GlideCloudTrailMist2.part
		part	:GlideCloudTrail2.part
		part	:GlideCloudFlatLocalTrail2.part
		part	:GlideCloudTrailFlat2.part
		part	:GlideCloudFlatLocal2.part
		part	:GlideMist2.part
		part	:GlideCloud2.part
		part	:SpeedStreaks2.part
		               
	End

End

End				